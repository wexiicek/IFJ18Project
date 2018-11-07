#ifndef _PARSER_H
#define _PARSER_H


#include <stdbool.h>

#include "symtable.h"
#include "scanner.h"


//#define GENERATE_CODE(_callback, ...)								
//	if (!_callback(__VA_ARGS__)) return INTERNAL


/**
 * @struct Parser's internal data representation.
 */
typedef struct
{
	Sym_table global_table;		/// Global symbol table
	Sym_table local_table;		/// Local symbol table

	Token token;				/// Token

	TData* current_id;			/// ID of currently processed function
	TData* lhs_id;				/// ID of left-hand-side variable
	TData* rhs_id;				/// ID of right-hand-side function (expression?)

	unsigned param_index;		/// Index of currently checked param
	int label_index;			/// Index for generating unique labels.
	int label_deep;				/// Deep of labes.

	bool in_function;			/// Defines if the parser is in function
	bool in_declaration;		/// Defines if param rule should add or check it's params
	bool in_while_or_if;		/// Defines if the parser is in construction while, if or then
	bool non_declared_function;	/// Function that has been only defined
} PData;


/**
 * Starts syntactic and semantic anlysis.
 *
 * @return Appropriate error code.
 */
int analyse();


#endif //_Parser_H