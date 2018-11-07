#include "scanner.h"
#include "expression.h"
#include "symstack.h"     //TODO   Pridat to Hikesove
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
	{ S , S , N , S , R , S , R }, // r (realtion operators) = <> <= < >= >
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
        return MORE; 
    case tokenLessEqual:
        return LESS_OR_EQUAL; 
    case tokenGreaterEqual:
        return MORE_OR_EQUAL;
    case tokenLeftBracket:
        return LEFT_BRACKET;
    case tokenRightBracket:
        return RIGHT_BRACKET;
    case token:      //TODO FOR ID
        return PLUS;
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


int expression(PData* data){
    int result = SYNTACTICAL;
    precAnalysisTableSymbol actualSymbol;


    do{
        actualSymbol = getSymbolFromToken(&data->token);
    }
}