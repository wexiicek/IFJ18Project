/*/////////////////////////////////////////////////////////////
/                                                             /                           
/    Subject: IFJ                                             /                           
/    Task: IFJ Project                                        /                           
/                                                             /                           
/          Author 1 (leader)               Author 2           /      
/           Marian Pukancik               Adam Linka          /                        
/              xpukan01                    xlinka01           /                     
/                                                             /                           
/    Team: 045, Variant I                                     /                           
/                                                             /                           
/////////////////////////////////////////////////////////////*/

#include "scanner.h"
#include "expression.h"
#include "stack.h"     
#include "err.h"
#include "generator.h"
#include "symtable.h"

#define tableSize 7
//We will work with this stack for the whole time
Stack stack;
Token token;


// 2-dimensional array
int precTable[tableSize][tableSize] =
{
    // < SHIFT
	// = EQUAL
	// > REDUCE
	// # ERROR
//	|+- | */| r | ( | ) | i | $ |
	{ R , S , R , S , R , S , R }, // +-
	{ R , R , R , S , R , S , R }, // */
	{ S , S , N , S , R , S , R }, // r (relation operators) = != <= < >= >
	{ S , S , S , S , E , S , N }, // (
	{ R , R , R , N , R , N , R }, // )
	{ R , R , R , N , R , N , R }, // i (id, int, double, string)
	{ S , S , S , S , N , S , N }  // $
};

FILE *dest;
/**
 * Function converts token type to symbol from precAnalysisTableSymbol.
 *
 * @param token  -  Pointer to token
 * @return       -  Function returns symbol according current token, otherwise it returns symbol dollar
 */
static precAnalysisTableSymbol getSymbolFromToken(Token* token){
	switch (token->Type)
	{
	case tokenAdd:
        return PLUS;
    case tokenSub:
        return MINUS; 
    case tokenMul:
        return MUL; 
    case tokenDiv:
        return DIV; 
    case tokenAssign:
        return ASSIGN; 
    case tokenEqual:
        return EQUAL; 
    case tokenNotEqual:
        return NOT_EQUAL; 
    case tokenLess:
        return LESS; 
    case tokenGreater:
        return GREATER; 
    case tokenLessEqual:
        return LESS_OR_EQUAL; 
    case tokenGreaterEqual:
        return GREATER_OR_EQUAL;
    case tokenLeftBracket:
        return LEFT_BRACKET;
    case tokenRightBracket:
        return RIGHT_BRACKET;
    case tokenIdentifier:    
        return IDENTIFIER;
    case tokenString:
        return STRING;
    case tokenFloat:
        return FLOAT_NUMBER;
    case tokenInteger:
        return INT_NUMBER;  
	default:
        return DOLLAR;
	}
}

/**
 * Function finds out data type of the token.
 *
 * @param token       -  Pointer to a  token
 * @param parserData  -  Pointer to parser's internal data
 * @return            -  Returns data type of actual token
 */
static dataTypeEnum getDataType(Token* token, parseData* parserData){
	tData* symbol;
    //If we get an identifier
	if(token->Type == tokenIdentifier){
        if (parserData->lID->global){               //If this item has parameter global from symtable set to true
            symbol = BSTsearchSymbol(parserData->globalTable, token->Data.string->value);
            if (symbol == NULL)                     //Then we will look for him in global table
                return TYPE_UNDEFINED;
            return symbol->dataType;
        }
        else{                                       //If not, then we will look in local table
        symbol = BSTsearchSymbol(parserData->localTable, token->Data.string->value);
                                                    //If we will not find anything, we will return TYPE_UNDEFINED
        if (symbol == NULL){
                    return TYPE_UNDEFINED;}
        return symbol->dataType;
        }
	}
    //If we get number or string
	else if(token->Type == tokenInteger)
		return TYPE_INTEGER;
	
	else if(token->Type == tokenFloat)
		return TYPE_FLOAT;

	else if(token->Type == tokenString)
		return TYPE_STRING;

	else return TYPE_UNDEFINED;
}

/**
 * Function find out how many symbols had beed pushed into stack after symbol stop
 * 
 * @param stopFound  -  Bool value which tells us if we found stop symbol
 * @return           -  Number of symbols after stop symbol
 */
static int numberOfSymbolsAfterStop(bool* stopFound){
	StackItem* tmp = stackTop(&stack);
	int count = 0;
	while (tmp != NULL){
		if (tmp->symbol != STOP){
			*stopFound = false;
			count++;
		}
		else{
			*stopFound = true;
			break;
		}
		tmp = tmp->nextPtr;
	}
	return count;
}

/**
 * Function converts symbol to precedence table index.
 *
 * @param symbol     -  Symbol to be converted
 * @return           -  Returns precedence table index
 */
static precTabIndexSymbol getPrecTableIndex(precAnalysisTableSymbol symbol){
    //These indexes will be used for searching in precedence table
    if((symbol == PLUS) || (symbol == MINUS))
        return I_PLUS_MINUS;
    else if((symbol == MUL) || (symbol == DIV))
        return I_MUL_DIV;
    else if((symbol == ASSIGN) || (symbol == EQUAL) || (symbol == LESS) || (symbol == GREATER) || (symbol == LESS_OR_EQUAL) || (symbol == GREATER_OR_EQUAL))
        return I_REL_OP;
    else if((symbol == LEFT_BRACKET))
        return I_LEFT_BRACKET;
    else if((symbol == RIGHT_BRACKET))
        return I_RIGHT_BRACKET;
    else if((symbol == IDENTIFIER) || (symbol == INT_NUMBER) || (symbol == FLOAT_NUMBER) || (symbol == STRING))
        return  I_ID;
    else return I_DOLLAR;
}

/**
 * Function tests if symbols in parameters are valid according to rules.
 *
 * @param numberOfOperands  -  Number of valid symbols in parameter
 * @param operand1          -  Symbol 1
 * @param operand2          -  Symbol 2
 * @param operand3          -  Symbol 3
 * @return                  -  If correct rule was not found, function returns NOT_A_RULE
 */
static precAnalysisRules testWhichRuleToUse(int numberOfOperands, StackItem *operand1, StackItem *operand2, StackItem *operand3){
    if (numberOfOperands == 1){   
		if (operand1->symbol == INT_NUMBER || operand1->symbol == FLOAT_NUMBER || operand1->symbol == STRING || operand1->symbol == IDENTIFIER)
			return OPERAND_RULE;                        // rule E -> i
        else
		    return NOT_A_RULE;                          // rule doesn't exist
    }

	else if(numberOfOperands == 3){
		if (operand1->symbol == NON_TERMINAL && operand3->symbol == NON_TERMINAL){
			if(operand2->symbol == PLUS)                // rule E -> E + E
                return PLUS_RULE;
            else if(operand2->symbol == MINUS)          // rule E -> E - E
                return MINUS_RULE;
            else if(operand2->symbol == MUL)            // rule E -> E * E
                return MUL_RULE;
            else if(operand2->symbol == DIV){           // rule E -> E / E
                if(operand1->dataType == TYPE_INTEGER && operand3->dataType == TYPE_INTEGER){
                    return IDIV_RULE;                   //IDIV for two integers, otherwise only DIV
                }
                else return DIV_RULE;
            }            
            else if(operand2->symbol == EQUAL)          // rule E -> E == E
                return EQUAL_RULE;
            else if(operand2->symbol == NOT_EQUAL)      // rule E -> E != E
                return NOT_EQUAL_RULE;
            else if(operand2->symbol == GREATER)        // rule E -> E > E
                return GREATER_RULE;
            else if(operand2->symbol == LESS)           // rule E -> E < E
                return LESS_RULE;  
            else if(operand2->symbol == GREATER_OR_EQUAL)  // rule E -> E >= E
                return GREATER_OR_EQUAL_RULE;
            else if(operand2->symbol == LESS_OR_EQUAL)  // rule E -> E <= E
                return LESS_OR_EQUAL_RULE;
            else
                return NOT_A_RULE;
        }
        else if(operand1->symbol == LEFT_BRACKET && operand2->symbol == NON_TERMINAL && operand3->symbol == RIGHT_BRACKET){
            return BRACKETS_RULE;                       // rule E -> (E)
        }
        else{
            return NOT_A_RULE;
        }
    }
    else
        return NOT_A_RULE;
}

/**
 * Function check Semantics of the expression 
 * 
 * @param rule        -  Rule from the function testWhichRuleToUse
 * @param op1         -  Left number, string or identifier
 * @param op2         -  Operator
 * @param op3         -  Right number, string or identifier
 * @param final_type  -  Final type of the expression
 * @param parserData  -  Pointer parser's internal data
 * @return            -  Final type if the semantics is right or correct error
 */
static int checkSemantics(precAnalysisRules rule, StackItem* op1, StackItem* op2, StackItem* op3, dataTypeEnum* final_type, parseData *parserData){
    
    //For us to know if we need to retype operands
    bool op1ToFloat = false;
	bool op3ToFloat = false;

    //If we are checking function. Because we ifj18 is dynamically typed language.
    //And we do not know data types of the operands used in functions.
    if (parserData -> inFunction == false && parserData -> inPrint == false){
        if (rule == OPERAND_RULE){
            if (op1->dataType == TYPE_UNDEFINED)
                return SEMANTICAL_UNDEF; 
        }

        if (rule == BRACKETS_RULE){
            if (op2->dataType == TYPE_UNDEFINED)
                return SEMANTICAL_UNDEF;
        }

        if (rule != OPERAND_RULE && rule != BRACKETS_RULE){
            if (op1->dataType == TYPE_UNDEFINED && op3->dataType == TYPE_UNDEFINED)
                return SEMANTICAL_UNDEF;
        }
    }

    switch (rule){
    case OPERAND_RULE:
        *final_type = op1->dataType;
        if (parserData->lID != NULL)
           parserData->lID->dataType = *final_type;
        break;

    case BRACKETS_RULE:
        *final_type = op2->dataType;
        if (parserData->lID != NULL)
            parserData->lID->dataType = *final_type;
        break;

    /* 
     * Here we can do concatenation of two strings.
     * We cal add two integers, floats 
     * Or if we have one operand integer and second float, we can retype integer to float
     */ 
    case PLUS_RULE:
        if (op1->dataType == TYPE_INTEGER && op3->dataType == TYPE_INTEGER){
            *final_type = TYPE_INTEGER;

            if (parserData->lID != NULL)
                parserData->lID->dataType = TYPE_INTEGER;
            break;
        }
        else if(op1->dataType == TYPE_FLOAT && op3->dataType == TYPE_FLOAT){
            *final_type = TYPE_FLOAT;

            if (parserData->lID != NULL)
                parserData->lID->dataType = TYPE_FLOAT;
            break;
        }
        else if(op1->dataType == TYPE_FLOAT && op3->dataType == TYPE_INTEGER){
            op3->dataType = TYPE_FLOAT;
            op3ToFloat = true;
            *final_type = TYPE_FLOAT;

            if (parserData->lID != NULL)
                parserData->lID->dataType = TYPE_FLOAT;
            break;
        }
        else if(op1->dataType == TYPE_INTEGER && op3->dataType == TYPE_FLOAT){
            op1->dataType = TYPE_FLOAT;
            op1ToFloat = true;
            *final_type = TYPE_FLOAT;

            if (parserData->lID != NULL)
                parserData->lID->dataType = TYPE_FLOAT;
            break;
        }
        else if (op1->dataType == TYPE_STRING && op3->dataType == TYPE_STRING && rule == PLUS_RULE){
            *final_type = TYPE_STRING;

            if (parserData->lID != NULL)
                parserData->lID->dataType = TYPE_STRING;
            break;
        }
        else if (parserData -> inFunction == true && (op1 -> dataType == TYPE_UNDEFINED || op3 -> dataType == TYPE_UNDEFINED))
        {
            *final_type = TYPE_UNDEFINED;
            break;
        }
        else 
            return SEMANTICAL_TYPES;
        break;

    /* 
     * Here we cannot do concatenation of two strings.
     * We cal add two integers, floats 
     * Or if we have one operand integer and second float, we can retype integer to float
     */ 

    case MINUS_RULE:
    case MUL_RULE:
    case DIV_RULE:
        if (op1->dataType == TYPE_INTEGER && op3->dataType == TYPE_INTEGER){
            *final_type = TYPE_INTEGER;
            parserData->lID->dataType = TYPE_INTEGER;
            break;
        }
        else if (op1->dataType == TYPE_STRING || op3->dataType == TYPE_STRING){
            return SEMANTICAL_TYPES;
            break;
        }
        else if(op1->dataType == TYPE_FLOAT && op3->dataType == TYPE_FLOAT){
            *final_type = TYPE_FLOAT;
            parserData->lID->dataType = TYPE_FLOAT;
            break;
        }
        else if(op1->dataType == TYPE_FLOAT && op3->dataType == TYPE_INTEGER){
            op3->dataType = TYPE_FLOAT;
            op3ToFloat = true;
            *final_type = TYPE_FLOAT;
            parserData->lID->dataType = TYPE_FLOAT;
            break;
        }
        else if(op1->dataType == TYPE_INTEGER && op3->dataType == TYPE_FLOAT){
            op1->dataType = TYPE_FLOAT;
            op1ToFloat = true;
            *final_type = TYPE_FLOAT;
            parserData->lID->dataType = TYPE_FLOAT;
            break;
        }
        else if (parserData -> inFunction == true && (op1 -> dataType == TYPE_UNDEFINED || op3 -> dataType == TYPE_UNDEFINED))
        {
            *final_type = TYPE_UNDEFINED;
            break;
        }
        else return SEMANTICAL_TYPES;
        break;

    case EQUAL_RULE:
    case NOT_EQUAL_RULE:
        if(op1->dataType == op3->dataType){
            *final_type = TYPE_UNDEFINED;
            break;
        }
        else return SEMANTICAL_TYPES;
        break;

    case IDIV_RULE:
        *final_type = TYPE_INTEGER;
        parserData->lID->dataType = TYPE_INTEGER;
        break;

    case LESS_OR_EQUAL_RULE:
    case LESS_RULE:
    case GREATER_OR_EQUAL_RULE:
    case GREATER_RULE:
        if(op1->dataType == op3->dataType){
            *final_type = TYPE_UNDEFINED;
            break;
        }
        else if(op1->dataType == TYPE_FLOAT && op3->dataType == TYPE_INTEGER){
            op3->dataType = TYPE_FLOAT;
            op3ToFloat = true;
            *final_type = TYPE_UNDEFINED;
            break;
        }
        else if(op1->dataType == TYPE_INTEGER && op3->dataType == TYPE_FLOAT){
            op1->dataType = TYPE_FLOAT;
            op1ToFloat = true;
            *final_type = TYPE_UNDEFINED;
            break;
        }
        else if (parserData -> inFunction == true && (op1 -> dataType == TYPE_UNDEFINED || op3 -> dataType == TYPE_UNDEFINED))
        {
            *final_type = TYPE_UNDEFINED;
            break;
        }
        else return SEMANTICAL_TYPES;
        break;

    default:
        break;
    }

    //If the bool variable is set to true, then we will retype integer to float
    if(op1ToFloat){
        addToOutput(codeGenOperand1toFloat,);
    }
    if(op3ToFloat){
        addToOutput(codeGenOperand3toFloat,);
    }
    return SUCCESS;
}


/**
 * Function reduces symbols after STOP symbol if rule for reducing was found.
 *
 * @param parserData     -  Pointer to table.
 * @return               -  Given exit code.
 */
static int reduceByRule(parseData* parserData){
	(void) parserData;
    int result;

	StackItem* op1 = NULL;
	StackItem* op2 = NULL;
	StackItem* op3 = NULL;

    precAnalysisRules rule;
	dataTypeEnum finalType;
	bool found = false;
    //We need to find out with how many operands are we dealing with
	int count = numberOfSymbolsAfterStop(&found);
    //For one operand 
    if(count == 1){
        if(found == true){
            op1 = stack.topPtr;
		    rule = testWhichRuleToUse(count, op1, NULL, NULL);
        }
    }
    //For three operands
    //Operands are saved on stack
	else if (count == 3){
        if(found == true){
            op1 = stack.topPtr->nextPtr->nextPtr;
            op2 = stack.topPtr->nextPtr;
            op3 = stack.topPtr;
            rule = testWhichRuleToUse(count, op1, op2, op3);
        }
	}
	else return SYNTACTICAL;

    //If do not find correct rule
	if (rule == NOT_A_RULE)
		return SYNTACTICAL;
	else{
		if ((result = checkSemantics(rule, op1, op2, op3, &finalType, parserData)))
            return result;
    
		if (rule == PLUS_RULE && finalType == TYPE_STRING)
			addToOutput(codeGenConcatStackStrings,);
	
		else addToOutput(codeGenStackOperation,rule);
        
		stackPopCount(&stack, count + 1);
		stackPush(&stack, NON_TERMINAL, finalType, token);
	}
    
	return SUCCESS;
}

/**
 * Implementation of <expression> rule. Parses expressions.
 *
 * @param parserData  -  Pointer to parser's internal data.
 * @return            -  Given exit code.
 */
//This function is the brain of whole precedence analysis
int expression(parseData *parserData){
    bool success = false;
    int result = SYNTACTICAL;
    stackInit(&stack);

    //At first we have to push dollar into stack, so we know where to end analysis
    int continue0 = stackPush(&stack, DOLLAR, TYPE_UNDEFINED, token);
    
    if(continue0 == false){
        stackFree(&stack);
        return INTERNAL;
    }

    StackItem *topTerminal;
    precAnalysisTableSymbol actualSymbol;

    //This part is direct implementation of algorithm from lecture
    do {
        actualSymbol = getSymbolFromToken(&parserData->token);
        topTerminal = stackTopTerminal(&stack);

        if(topTerminal == NULL){
            stackFree(&stack);
            return INTERNAL;
        }
        
        //If we get symbol "<" from symtable 
        if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == S){
            int continue1 = symbolStackInsertAfterTopTerminal(&stack, STOP, TYPE_UNDEFINED);
            if (continue1 == false){
                stackFree(&stack);
                return INTERNAL;
            }

            int continue2 = stackPush(&stack, actualSymbol, getDataType(&parserData->token, parserData), token);
            if (continue2 == false){
                stackFree(&stack);
                return INTERNAL;
            }

            if(actualSymbol == IDENTIFIER || actualSymbol == INT_NUMBER || actualSymbol == FLOAT_NUMBER || actualSymbol == STRING){
                addToOutput(codeGenPush, (*parserData));
            }

            result = getTokens(&parserData->token);
            if (result){
                stackFree(&stack);
                return result;
            }
        }

        //If we get symbol "=" from symtable
        else if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == E){
            stackPush(&stack, actualSymbol, getDataType(&parserData->token, parserData), token);

            result = getTokens(&parserData->token);
            if (result){
                stackFree(&stack);
                return result;
            }
        }

        //If we get symbol ">" from symtable
        else if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == R){
            result = reduceByRule(parserData);
            if (result){
                stackFree(&stack);
                return result;
            }
        }

        //If we get symbol "error" from symtable
        else if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == N){
            if ((actualSymbol == DOLLAR) && (topTerminal->symbol == DOLLAR)){
                success = true;
            }
            else{
                stackFree(&stack);
                return SYNTACTICAL;
            }
        }

    } while (!success);

    StackItem *finalNT = stackTop(&stack);
    if(finalNT == NULL) {
        stackFree(&stack);
        return INTERNAL;
    }
    if(finalNT -> symbol != NON_TERMINAL) {
        stackFree(&stack);
        return SYNTACTICAL;
    }

    //In this section we will generate code
    if(parserData->lID != NULL){
        char *frame = "LF";  //LF = local frame
        if(parserData->lID->global){
            frame = "GF";    //GF = global frame
        }

        //We will call generator's functions depending on data types
        //To be more specific depending on data type of the left side if the expression and data type of the final nonterminal
        switch(parserData->lID->dataType){
            case TYPE_INTEGER:
                if(finalNT->dataType == TYPE_STRING){
                    stackFree(&stack);
                    return SEMANTICAL_TYPES;
                }      

                addToOutput(codeGenSaveExpressionResult,parserData->lID->identifier,frame,TYPE_INTEGER,finalNT->dataType);
                break;
            case TYPE_FLOAT:
                if(finalNT->dataType == TYPE_STRING){
                    stackFree(&stack);
                    return SEMANTICAL_TYPES;
                }
                addToOutput(codeGenSaveExpressionResult,parserData->lID->identifier,frame,TYPE_FLOAT,finalNT->dataType);
                break;
            case TYPE_STRING:
                if(finalNT->dataType != TYPE_STRING){
                    stackFree(&stack);
                    return SEMANTICAL_TYPES;
                }
                addToOutput(codeGenSaveExpressionResult,parserData->lID->identifier,frame,TYPE_STRING,TYPE_STRING);
                break;
            case TYPE_UNDEFINED:
                addToOutput(codeGenSaveExpressionResult,parserData->lID->identifier,frame,TYPE_UNDEFINED,finalNT->dataType);
                break;
            default:
			break;
        }
    }
    stackFree(&stack);
    return SUCCESS;
}
