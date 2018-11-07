#ifndef _EXPRESSION_H
#define _EXPRESSION_H


#include "parser.h"


/**
 * @enum Rules used for parsing.
 */
typedef enum
{
	NT_EQ_NT,		// E -> E = E
	NT_NEQ_NT,		// E -> E <> E
	NT_LEQ_NT,		// E -> E <= E
	NT_LTN_NT,		// E -> E < E
	NT_MEQ_NT,		// E -> E => E
	NT_MTN_NT,		// E -> E > E
	NT_PLUS_NT,		// E -> E + E
	NT_MINUS_NT,	// E -> E - E
	NT_IDIV_NT,		// E -> E \ E
	NT_MUL_NT,		// E -> E * E
	NT_DIV_NT,		// E -> E / E
	LBR_NT_RBR,		// E -> (E)
	OPERAND,		// E -> i
	NOT_A_RULE		// rule doesn't exist
} precAnalysisRules;

/**
 * @enum Symbols used for precednece analysis.
 */
typedef enum
{
	PLUS,			// +
	MINUS,			// -
	MUL,			// *
	DIV,			// /
	//IDIV,			// \ /    //TODO Nevim, ci to tam mame
	ASSIGN          // =
    EQUAL,	        // ==
	NOT_EQUAL,		// !=
	LESS_OR_EQUAL,	// <=
	LESS,			// <
	MORE_OR_EQUAL,	// >=
	MORE,			// >
	LEFT_BRACKET,	// (
	RIGHT_BRACKET,	// )
	IDENTIFIER,		// ID
	INT_NUMBER,		// int
	FLOAT_NUMBER,	// float
	STRING,			// string
	DOLLAR,			// $
	STOP,			// stop symbol used when reducing
	NON_TERMINAL	// non-terminal
}precAnalysisTableSymbol;


/**
 * Implementation of <expression> rule. Parses expressions.
 *
 * @param data Pointer to parser's internal data.
 * @return Given exit code.
 */
int expression(PData* data);


#endif //_parser_H