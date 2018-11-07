#include <stdbool.h>
//#include "symtable.h"

/**
 * Toto sem iba ujebal od Harmima.
 * @enum Symbols used for precednece analysis.
 */
typedef enum
{
	PLUS,			/// +
	MINUS,			/// -
	MUL,			/// *
	DIV,			/// /
	IDIV,			/// \ /
	EQ,				/// =
	NEQ,			/// <>
	LEQ,			/// <=
	LTN,			/// <
	MEQ,			/// >=
	MTN,			/// >
	LEFT_BRACKET,	/// (
	RIGHT_BRACKET,	/// )
	IDENTIFIER,		/// ID
	INT_NUMBER,		/// int
	DOUBLE_NUMBER,	/// double
	STRING,			/// string
	DOLLAR,			/// $
	STOP,			/// stop symbol used when reducing
	NON_TERM		/// non-terminal
} Prec_table_symbol_enum;

/**
 * @enum Data types.
 */
typedef enum
{
	TYPE_UNDEFINED,	/// Data type undefined
	TYPE_INT,		/// Integer data type
	TYPE_DOUBLE,	/// Double data type
	TYPE_STRING,	/// String data type
	TYPE_BOOL,		/// Bool data type (actaully it's kinda imaginary)
} Data_type;

/**
 * Taktez iba ujebane od Harmima.
 * @struct Stack item represetation.
 */
typedef struct item
{
	Prec_table_symbol_enum symbol; // U Harmima v expression.h
	Data_type data_type; // U Harmima v symtable.h
	struct item *nextPtr; /// Pointer to next stack item.
} StackItem;

typedef struct {
	StackItem* topPtr;
} Stack;

void stackInit(Stack* stack);
bool stackPush(Stack* stack, Prec_table_symbol_enum symbol, Data_type type);
bool stackPop(Stack* stack);
StackItem* stackTop(Stack *stack);
void stackFree(Stack* stack);
void stackPopCount(Stack* stack, int count);
StackItem* stackTopTerminal(Stack* stack);
bool symbolStackInsertAfterTopTerminal(Stack* stack, Prec_table_symbol_enum symbol, Data_type type);