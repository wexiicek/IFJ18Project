#include "scanner.h"
#include "expression.h"
#include "stack.h"     
#include "err.h"
#include "generator.h"
#include "symtable.h"



#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"

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
	{ S , S , N , S , R , S , R }, // r (realtion operators) = != <= < >= >
	{ S , S , S , S , E , S , N }, // (
	{ R , R , R , N , R , N , R }, // )
	{ R , R , R , N , R , N , R }, // i (id, int, double, string)
	{ S , S , S , S , N , S , N }  // $
};

FILE *dest;

/**
 * Function converts token type to symbol.
 *
 * @param token Pointer to token.
 * @return Returns dollar if symbol is not supported or converted symbol if symbol is supported.
 */
static precAnalysisTableSymbol getSymbolFromToken(Token* token)
{
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
 * Function converts token type to data type.
 *
 * @param token Pointer to token.
 * @param data Pointer to parser's internal data.
 * @return Returns data type of actual token.
 */
static dataTypeEnum getDataType(Token* token, parseData* parserData){
	tData* symbol;

	if(token->Type == tokenIdentifier){
		symbol = BSTsearchSymbol(parserData->localTable, token->Data.string->value);
		if (symbol == NULL)
			return TYPE_UNDEFINED;
		return symbol->dataType;
	}
	else if(token->Type == tokenInteger)
		return TYPE_INTEGER;
	
	else if(token->Type == tokenFloat)
		return TYPE_FLOAT;

	else if(token->Type == tokenString)
		return TYPE_STRING;

	else return TYPE_UNDEFINED;
}

static int numberOfSymbolsAfterStop(bool* stopFound)
{
    fprintf(stderr,CGRN"    [EXPR]" CRED" FUN "CWHT" numberOfSymbolsAfterStop \n");
	StackItem* tmp = stackTop(&stack);
    //printStack(&stack);
	int count = 0;
//for debugging purposes
    //return count;
//debug TODO
	while (tmp != NULL){
		if (tmp->symbol != STOP){
			*stopFound = false;
			count++;

            //if (count == 20) return;
            //debug
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
 * @param symbol Symbol to be converted.
 * @return Returns precedence table index.
 */
static precTabIndexSymbol getPrecTableIndex(precAnalysisTableSymbol symbol){
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
 * @param num Number of valid symbols in parameter.
 * @param operand1 Symbol 1.
 * @param operand2 Symbol 2.
 * @param operand3 Symbol 3.
 * @return NOT_A_RULE if no rule is found or returns rule which is valid.
 */
static precAnalysisRules testWhichRuleToUse(int numberOfOperands, StackItem *operand1, StackItem *operand2, StackItem *operand3){
	fprintf(stderr, CGRN"    [EXPR]"CWHT" testWhichRuleToUse.. Operand count: %d\n", numberOfOperands);

    if (numberOfOperands == 1){
		// rule E -> i
		if (operand1->symbol == INT_NUMBER || operand1->symbol == FLOAT_NUMBER || operand1->symbol == STRING || operand1->symbol == IDENTIFIER)
			{fprintf(stderr, CGRN"    [EXPR]"CRED" RET "CWHT"OPERAND RULE\n");return OPERAND_RULE;}
        else
		    return NOT_A_RULE;
    }

	else if(numberOfOperands == 3){
		if (operand1->symbol == NON_TERMINAL && operand3->symbol == NON_TERMINAL){
			if(operand2->symbol == PLUS)                // rule E -> E + E
                return PLUS_RULE;
            else if(operand2->symbol == MINUS)          // rule E -> E - E
                return MINUS_RULE;
            else if(operand2->symbol == MUL)            // rule E -> E * E
                return MUL_RULE;
            else if(operand2->symbol == DIV)            // rule E -> E / E
                return DIV_RULE;
            else if(operand2->symbol == EQUAL)          // rule E -> E = E
                return EQUAL_RULE;
            else if(operand2->symbol == NOT_EQUAL)      // rule E -> E != E
                return NOT_EQUAL_RULE;
            else if(operand2->symbol == GREATER)       // rule E -> E > E
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
        else
            {return NOT_A_RULE;}
    }
    else
        return NOT_A_RULE;
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
static int checkSemantics(precAnalysisRules rule, StackItem* op1, StackItem* op2, StackItem* op3, dataTypeEnum* final_type){
    bool op1ToFloat = false;
	bool op3ToFloat = false;
    
    if (rule == OPERAND_RULE)
    {
        if (op1->dataType == TYPE_UNDEFINED)
            return SEMANTICAL_OTHER;
    }

    if (rule == BRACKETS_RULE)
    {
        if (op2->dataType == TYPE_UNDEFINED)
            return SEMANTICAL_OTHER;
    }

    if (rule != OPERAND_RULE && rule != BRACKETS_RULE)
    {
        if (op1->dataType == TYPE_UNDEFINED || op3->dataType == TYPE_UNDEFINED)
            return SEMANTICAL_OTHER;
    }

    switch (rule)
    {
    case OPERAND_RULE:
        *final_type = op1->dataType;
        break;

    case BRACKETS_RULE:
        *final_type = op2->dataType;
        break;

    case PLUS_RULE:
        if (op1->dataType == TYPE_INTEGER && op3->dataType == TYPE_INTEGER){
            *final_type = TYPE_INTEGER;
            break;
        }
        else if(op1->dataType == TYPE_FLOAT && op3->dataType == TYPE_FLOAT){
            *final_type = TYPE_FLOAT;
            break;
        }
        else if(op1->dataType == TYPE_FLOAT && op3->dataType == TYPE_INTEGER){
            op3->dataType = TYPE_FLOAT;
            op3ToFloat = true;
            *final_type = TYPE_FLOAT;
            break;
        }
        else if(op1->dataType == TYPE_INTEGER && op3->dataType == TYPE_FLOAT){
            op1->dataType = TYPE_FLOAT;
            op1ToFloat = true;
            *final_type = TYPE_FLOAT;
            break;
        }
        else if (op1->dataType == TYPE_STRING && op3->dataType == TYPE_STRING && rule == PLUS_RULE){
            *final_type = TYPE_STRING;
            break;
        }
        else return SEMANTICAL_OTHER;
        break;
    case MINUS_RULE:
    case MUL_RULE:
    case DIV_RULE:
        if (op1->dataType == TYPE_INTEGER && op3->dataType == TYPE_INTEGER){
            *final_type = TYPE_INTEGER;
            break;
        }
        else if (op1->dataType == TYPE_STRING || op3->dataType == TYPE_STRING){
            return SEMANTICAL_OTHER;
            break;
        }
        else if(op1->dataType == TYPE_FLOAT && op3->dataType == TYPE_FLOAT){
            *final_type = TYPE_FLOAT;
            break;
        }
        else if(op1->dataType == TYPE_FLOAT && op3->dataType == TYPE_INTEGER){
            op3->dataType = TYPE_FLOAT;
            op3ToFloat = true;
            *final_type = TYPE_FLOAT;
            break;
        }
        else if(op1->dataType == TYPE_INTEGER && op3->dataType == TYPE_FLOAT){
            op1->dataType = TYPE_FLOAT;
            op1ToFloat = true;
            *final_type = TYPE_FLOAT;
            break;
        }
        else return SEMANTICAL_OTHER;
        break;
    case EQUAL_RULE:
    case NOT_EQUAL_RULE:
        if(op1->dataType == op3->dataType){
            *final_type = TYPE_UNDEFINED;
            break;
        }
        else return SEMANTICAL_OTHER;
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
        else return SEMANTICAL_OTHER;
        break;

    default:
        break;
    }

    if(op1ToFloat){
        addToOutput(codeGenOperand1toFloat,);
    }
    if(op3ToFloat){
        addToOutput(codeGenOperand3toFloat,);
    }
    return SUCCESS;
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/**
 * Function reduces symbols after STOP symbol if rule for reducing is found.
 *
 * @param data Pointer to table.
 * @return Given exit code.
 */
static int reduceByRule(parseData* parserData)
{
    fprintf(stderr,CGRN"    [EXPR]"CRED" FUN "CWHT" reduceByRule\n");
	(void) parserData;
    int result;

	StackItem* op1 = NULL;
	StackItem* op2 = NULL;
	StackItem* op3 = NULL;
	dataTypeEnum finalType;
	precAnalysisRules rule;
	bool found = false;
	int count = numberOfSymbolsAfterStop(&found);
   fprintf(stderr,CGRN"    [EXPR]"CWHT" numberOfSymbolsAfterStop %d\n", count);
    if(count == 1){
        if(found == true){
            op1 = stack.topPtr;
		    rule = testWhichRuleToUse(count, op1, NULL, NULL);
        }
    }
	else if (count == 3){
        if(found == true){
            op1 = stack.topPtr->nextPtr->nextPtr;
            op2 = stack.topPtr->nextPtr;
            op3 = stack.topPtr;
            rule = testWhichRuleToUse(count, op1, op2, op3);
        }
	}
	else {return SYNTACTICAL;}

	if (rule == NOT_A_RULE){
       fprintf(stderr,CGRN"    [EXPR]"CRED" RET "CWHT" NOT A RULE (SYNTACTICAL)\n");
		return SYNTACTICAL;}

	else{
		if ((result = checkSemantics(rule, op1, op2, op3, &finalType)))
            return result;
            

		if (rule == PLUS_RULE && finalType == TYPE_STRING)
		{
			addToOutput(codeGenConcatStackStrings,);
		}
		else addToOutput(codeGenStackOperation,rule);
        //printStack(&stack);
		stackPopCount(&stack, count + 1);
        //printStack(&stack);
		stackPush(&stack, NON_TERMINAL, finalType, token);

	}
    
	return SUCCESS;
}

int expression(parseData *parserData){
    bool success = false;
    int result = SYNTACTICAL;
    stackInit(&stack);

    int continue0 = stackPush(&stack, DOLLAR, TYPE_UNDEFINED, token);
    
    if(continue0 == false){
        stackFree(&stack);
        return INTERNAL;
    }

    StackItem *topTerminal;
    precAnalysisTableSymbol actualSymbol;

    do {
        actualSymbol = getSymbolFromToken(&parserData->token);
        topTerminal = stackTopTerminal(&stack);

        if(topTerminal == NULL){
            stackFree(&stack);
            return INTERNAL;
        }

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
                fprintf(stderr, "%s\n", "JEBEK\n");
                addToOutput(codeGenPush, (*parserData));
            }

            result = getTokens(&parserData->token);
            if (result){
                stackFree(&stack);
                return result;
            }
        }

        else if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == E){
            stackPush(&stack, actualSymbol, getDataType(&parserData->token, parserData), token);

            result = getTokens(&parserData->token);
            if (result){
                stackFree(&stack);
                return result;
            }
        }

        else if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == R){
            result = reduceByRule(parserData);
            if (result){
                stackFree(&stack);
                return result;
            }
        }

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
    if(finalNT == NULL) {stackFree(&stack);
     return INTERNAL;}
    if(finalNT -> symbol != NON_TERMINAL) {stackFree(&stack);
     return SYNTACTICAL;}

    if(parserData->lID != NULL){
        char *frame = "LF";
        if(parserData->lID->global){
            frame = "GF";
        }

        switch(parserData->lID->dataType){
            case TYPE_INTEGER:
                if(finalNT->dataType == TYPE_STRING){
                    stackFree(&stack);
                    return SEMANTICAL_OTHER;
                }
                fprintf(stderr, "%s\n", "KOKOT\n");
                addToOutput(codeGenSaveExpressionResult,parserData->lID->identifier,frame,TYPE_INTEGER,finalNT->dataType);
                break;
            case TYPE_FLOAT:
                if(finalNT->dataType == TYPE_STRING){
                    stackFree(&stack);
                    return SEMANTICAL_OTHER;
                }
                addToOutput(codeGenSaveExpressionResult,parserData->lID->identifier,frame,TYPE_FLOAT,finalNT->dataType);
                break;
            case TYPE_STRING:
                if(finalNT->dataType != TYPE_STRING){
                    stackFree(&stack);
                    return SEMANTICAL_OTHER;
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
