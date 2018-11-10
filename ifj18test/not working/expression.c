#include "scanner.h"
#include "expression.h"
#include "stack.h"     
#include "errors.h"
#include "generator.h"

#define tableSize 7

//TODO Nahradit funkciu od Hikesa
//#define FREE_RESOURCES_RETURN(_return)								
//	do {															
//		symbol_stack_free(&stack);									
//		return _return;												
//	} while(0)


Symbol_stack stack;

/**
 * @enum Precedence table symbols.
 */
typedef enum
{
	S,    // < SHIFT
	E,    // = EQUAL
	R,    // > REDUCE
	N     // # ERROR
} Prec_table_sign_enum;

/**
 * @enum Indexes for precedence table.
 */
typedef enum
{
	I_PLUS_MINUS,		// 0 +-
	I_MUL_DIV,			// 1 */
	I_REL_OP,			// 2 r
	I_LEFT_BRACKET,		// 3 (
	I_RIGHT_BRACKET,	// 4 )
	I_ID,				// 5 i
	I_DOLLAR			// 6 $
} precTabIndexSymbol;

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
        return  GREATHER; 
    case tokenLessEqual:
        return LESS_OR_EQUAL; 
    case tokenGreaterEqual:
        return GREATHER_OR_EQUAL;
    case tokenLeftBracket:
        return LEFT_BRACKET;
    case tokenRightBracket:
        return RIGHT_BRACKET;
    case token:      //TODO FOR ID
        return ;
    case tokenString:
        return STRING;
    case tokenFloat:
        return FLOAT_NUMBER;
    case tokenInteger:
        return INT_NUMBER;  
	default
        return DOLLAR;
	}
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
    else if((symbol == ASSIGN) || (symbol == EQUAL) || (symbol == LESS) || (symbol == MORE) || (symbol == LESS_OR_EQUAL) || (symbol == MORE_OR_EQUAL))
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
            else if(operand2->symbol == GREATHER)       // rule E -> E > E
                return GREATHER_RULE;
            else if(operand2->symbol == LESS)           // rule E -> E < E
                return LESS_RULE;  
            else if(operand2->symbol == GREATHER_OR_EQUAL)  // rule E -> E >= E
                return GREATHER_OR_EQUAL_RULE;
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


int expression(PData* data){
    int result = SYNTACTICAL;
    
    
    precAnalysisTableSymbol actualSymbol;


    do{
        actualSymbol = getSymbolFromToken(&data->token);
    }
}