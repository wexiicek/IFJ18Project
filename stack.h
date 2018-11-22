#ifndef STACK_HEADER
#define STACK_HEADER

#include <stdbool.h>
#include "expression.h"
#include "symtable.h"

/**
 * Taktez iba ujebane od Harmima.
 * Stack item represetation.
 */
typedef struct item
{
	precAnalysisTableSymbol symbol; 
	dataTypeEnum dataType; 
	struct item *nextPtr; /// Pointer to next stack item.
} StackItem;

typedef struct {
	StackItem  *topPtr;
} Stack;

void stackInit(Stack* stack);
bool stackPush(Stack* stack, precAnalysisTableSymbol symbol, dataTypeEnum type);
bool stackPop(Stack* stack);
StackItem* stackTop(Stack *stack);
void stackFree(Stack* stack);
void stackPopCount(Stack* stack, int count);
StackItem* stackTopTerminal(Stack* stack);
bool symbolStackInsertAfterTopTerminal(Stack* stack, precAnalysisTableSymbol symbol, dataTypeEnum type);

#endif