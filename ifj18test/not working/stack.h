#include <stdbool.h>
#include "symtable.h"

/**
 * Taktez iba ujebane od Harmima.
 * @struct Stack item represetation.
 */
typedef struct item
{
	precAnalysisTableSymbol symbol; 
	Data_type data_type; // U Harmima v symtable.h
	struct item *nextPtr; /// Pointer to next stack item.
} StackItem;

typedef struct {
	StackItem  *topPtr;
} Stack;

void stackInit(Stack* stack);
bool stackPush(Stack* stack, Prec_table_symbol_enum symbol, Data_type type);
bool stackPop(Stack* stack);
StackItem* stackTop(Stack *stack);
void stackFree(Stack* stack);
void stackPopCount(Stack* stack, int count);
StackItem* stackTopTerminal(Stack* stack);
bool symbolStackInsertAfterTopTerminal(Stack* stack, Prec_table_symbol_enum symbol, Data_type type);