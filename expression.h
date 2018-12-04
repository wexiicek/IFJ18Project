#ifndef _EXPRESSION_H
#define _EXPRESSION_H


#include "parser.h"


/**
 * @enum Rules used for parsing.
 */
typedef enum
{
	PLUS_RULE,		    	// E -> E + E
	MINUS_RULE,	        	// E -> E - E
	MUL_RULE,				// E -> E * E
	DIV_RULE,				// E -> E / E
	IDIV_RULE,				// E -> E / E
	EQUAL_RULE,				// E -> E = E
	NOT_EQUAL_RULE,			// E -> E <> E
	LESS_OR_EQUAL_RULE,		// E -> E <= E
	LESS_RULE,				// E -> E < E
	GREATER_OR_EQUAL_RULE,	// E -> E => E
	GREATER_RULE,			// E -> E > E
	BRACKETS_RULE,			// E -> (E)
	OPERAND_RULE,			// E -> i
	NOT_A_RULE				// rule doesn't exist
} precAnalysisRules;

/**
 * @enum Symbols used for precednece analysis.
 */
typedef enum
{
	PLUS,				// + 0
	MINUS,				// - 1 
	MUL,				// * 2
	DIV,				// / 3
	ASSIGN,          	// = 4
    EQUAL,	        	// == 5
	NOT_EQUAL,			// != 6
	LESS_OR_EQUAL,		// <= 7
	LESS,				// < 8
	GREATER_OR_EQUAL,	// >= 9
	GREATER,			// > 10 
	LEFT_BRACKET,		// ( 11 
	RIGHT_BRACKET,		// ) 12
	IDENTIFIER,			// ID 13 
	INT_NUMBER,			// int 14
	FLOAT_NUMBER,		// float 15
	STRING,				// string 16 
	DOLLAR,				// $ 17
	STOP,				// stop symbol used when reducing 18
	NON_TERMINAL		// non-terminal 19
}precAnalysisTableSymbol;

/**
 * @enum Precedence table symbols.
 */
typedef enum
{
	S,    // < SHIFT
	E,    // = EQUAL
	R,    // > REDUCE
	N     // # ERROR
} precTableSign;

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

/**
 * Implementation of <expression> rule. Parses expressions.
 *
 * @param data Pointer to parser's internal data.
 * @return Given exit code.
 */
int expression(parseData* parserData);


#endif //_parser_H