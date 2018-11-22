#include "scanner.h"
#include "expression.h"
#include "stack.h"     
#include "err.h"
//#include "generator.h"
#include "symtable.h"

#define tableSize 7
//We will work with this stack for the whole time
Stack stack;

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
	tBSTNodePtr symbol;

	if(token->Type == tokenIdentifier){
		symbol = symTableSearch(&parserData->localTable, token->Data.string->value);
		if (symbol == NULL)
			return TYPE_UNDEFINED;
		return (symbol)->nodeDataType;
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
 * @param op1 Symbol 1.
 * @param op2 Symbol 2.
 * @param op3 Symbol 3.
 * @return NOT_A_RULE if no rule is found or returns rule which is valid.
 */
static precAnalysisRules testWhichRuleToUse(int numberOfOperands, StackItem *operand1, StackItem *operand2, StackItem *operand3){
	
    if (numberOfOperands == 1){
		// rule E -> i
		if (operand1->symbol == INT_NUMBER || operand1->symbol == FLOAT_NUMBER || operand1->symbol == STRING || operand1->symbol == IDENTIFIER)
			return OPERAND_RULE;
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
            //else if(operand2->symbol == IDIV)         // rule E -> E \ E
            //    return IDIV_RULE;
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
            return NOT_A_RULE;
    }
    else
        return NOT_A_RULE;
}


/**
 * Function checks semantics of operands according to rule.
 *
 * @param rule Pointer to table.
 * @param op1 Symbol 1.
 * @param op2 Symbol 2.
 * @param op3 Symbol 3.
 * @param final_type Sets data type which will be after executing rule.
 * @return Given exit code.
 */
static int checkSemantics(precAnalysisRules rule, StackItem* operand1, StackItem* operand2, StackItem* operand3, dataTypeEnum* finalType){
	/*bool retypeToFloat1 = false;
	bool retypeToFloat3 = false;
	bool retypeToInt1 = false;
	bool retypeToInt3 = false;*/

    //What to do, if something had gone wrong
	if ((rule == OPERAND_RULE) || (rule == BRACKETS_RULE)){
		if (operand1->dataType== TYPE_UNDEFINED)
			return SEMANTICAL_OTHER;

		if (operand2->dataType == TYPE_UNDEFINED)
			return SEMANTICAL_OTHER;
	}

	if ((rule != OPERAND_RULE) && (rule != BRACKETS_RULE)){
		if (operand1->dataType == TYPE_UNDEFINED || operand3->dataType == TYPE_UNDEFINED)
			return SEMANTICAL_OTHER;
	}
    /*************************************************************************************/

	if(rule == OPERAND_RULE)
		*finalType = operand1->dataType;
		
	
	else if(rule == BRACKETS_RULE)
		*finalType = operand2->dataType;
	
	else if((rule == PLUS_RULE) || (rule == MINUS_RULE) || (rule == MUL_RULE)){
		if(operand1->dataType == TYPE_INTEGER){
			if(operand3->dataType == TYPE_INTEGER)
				*finalType = TYPE_INTEGER;
			
			else return SEMANTICAL_OTHER;
		}  
		
		if(operand1->dataType == TYPE_FLOAT){
			if(operand3->dataType == TYPE_FLOAT)
				*finalType = TYPE_FLOAT;
			
			else return SEMANTICAL_OTHER;
		}  

		if(operand1->dataType == TYPE_STRING){
			if(operand3->dataType == TYPE_STRING)
				*finalType = TYPE_STRING;
			
			else return SEMANTICAL_OTHER;
		}  
	}

	else if(rule == DIV_RULE){
		if(operand1->dataType == TYPE_FLOAT){
			if(operand3->dataType == TYPE_FLOAT){
				*finalType = TYPE_FLOAT; 
			}
			else return SEMANTICAL_OTHER;
		}

		if((operand1->dataType == TYPE_STRING) || (operand3->dataType == TYPE_STRING))
			return SEMANTICAL_OTHER;

		/**if(operand1->dataType == TYPE_INTEGER) //TODO KONZULTOVAT S OSTATNYMA
			retypeToFloat1 = true;
		
		if(operand3->dataType == TYPE_INTEGER)
			retypeToFloat3 = true;
		*/
		*finalType = TYPE_FLOAT;
	}

	/*if(rule == IDIV_RULE){

	}*/

	else if((rule == EQUAL_RULE) || (rule == NOT_EQUAL_RULE) || (rule == LESS_OR_EQUAL_RULE) || (rule == LESS_RULE) || (rule == GREATER_OR_EQUAL_RULE) || (rule == GREATER_RULE)){
		if(operand1->dataType != operand3->dataType)
			return SEMANTICAL_OTHER;
		
	}

	
	else return SUCCESS;
	return SUCCESS;
}


/**
 * Function reduces symbols after STOP symbol if rule for reducing is found.
 *
 * @param data Pointer to table.
 * @return Given exit code.
 */
static int reduceByRule(parseData* parserData)
{
	(void) parserData;
    int result;

	StackItem* op1 = NULL;
	StackItem* op2 = NULL;
	StackItem* op3 = NULL;
	dataTypeEnum finalType;
	precAnalysisRules rule;
	bool found = false;

	int count = numberOfSymbolsAfterStop(&found);

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
	else return SYNTACTICAL;

	if (rule == NOT_A_RULE)
		return SYNTACTICAL;

	else{
		if ((result = checkSemantics(rule, op1, op2, op3, &finalType)))
			return result;

		/*if (rule == PLUS_RULE && finalType == TYPE_STRING)
		{
			GENERATE_CODE(generate_concat_stack_strings);
		}
		else GENERATE_CODE(generate_stack_operation, rule_for_code_gen);*/

		stackPopCount(&stack, count + 1);
		stackPush(&stack, NON_TERMINAL, finalType);
	}
    
	return SUCCESS;
}



int expression(parseData* parserData){
    fprintf(stderr,"jebek exp\n");
    int result = SYNTACTICAL;
    bool resultDecider;
    
    stackInit(&stack);
    resultDecider = stackPush(&stack, DOLLAR, TYPE_UNDEFINED);
    if(resultDecider == false){
        stackFree(&stack); 
        return INTERNAL;
    };
    
    StackItem *topTerminal;
    precAnalysisTableSymbol actualSymbol;

    bool success = false;
    do{
        actualSymbol = getSymbolFromToken(&parserData->token);
        topTerminal = stackTop(&stack);
        if (topTerminal == NULL){
            stackFree(&stack);
            return INTERNAL;
        }

        if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == S){
            resultDecider = symbolStackInsertAfterTopTerminal(&stack, STOP, TYPE_UNDEFINED);
            if(resultDecider == false){
                stackFree(&stack);
                return INTERNAL;
            }

            resultDecider = stackPush(&stack,actualSymbol, getDataType(&parserData->token,parserData));
            if(resultDecider == false){
                stackFree(&stack);
                return INTERNAL;
            }

            if (actualSymbol == IDENTIFIER || actualSymbol == INT_NUMBER || actualSymbol == FLOAT_NUMBER || actualSymbol == STRING)
			{
				//resultDecider = generate_push(parserData->token);
                if(resultDecider == false){
                    stackFree(&stack);
                    return INTERNAL;
                }
			}

            if ((result = getTokens(&parserData->token)))
				stackFree(&stack);
                return result;
			break;
        }

        if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == E){
            stackPush(&stack, actualSymbol, getDataType(&parserData->token,parserData));
            
            if ((result = getTokens(&parserData->token)))
				stackFree(&stack);
                return result;
			break;
        }

        if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == R){
            
            if ((result = reduceByRule(parserData)))
				stackFree(&stack);
                return result;
			break;
        }

        if((precTable[getPrecTableIndex(topTerminal->symbol)][getPrecTableIndex(actualSymbol)]) == N){
            if(actualSymbol == DOLLAR){
                if(topTerminal->symbol == DOLLAR){
                    success = true;
                }
            }
            else{
                stackFree(&stack);
                return SYNTACTICAL;
            }
        }
    }while(success == false);
    return SUCCESS;
}