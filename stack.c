#include <stdlib.h>
#include "stack.h"

#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"

void stackInit(Stack* stack) {
	stack->topPtr = NULL;
}

bool stackPush(Stack* stack, precAnalysisTableSymbol symbol, dataTypeEnum type) {
	StackItem* newElemPtr = (StackItem *) malloc(sizeof(StackItem));
	if (newElemPtr == NULL) {
		return false;
	}
	newElemPtr->symbol = symbol;
	newElemPtr->dataType = type;
	newElemPtr->nextPtr = stack->topPtr;
	stack->topPtr = newElemPtr;
	return true;
} 

bool stackPop(Stack* stack) {
	StackItem *elemPtr;
	if (stack->topPtr != NULL) {
		elemPtr = stack->topPtr;
		stack->topPtr = stack->topPtr->nextPtr;
		free(elemPtr);
		return true;
	}
	return false;
}

StackItem* stackTop(Stack *stack) {
	return (stack->topPtr);
	//TODO možno dojebané
}

void stackFree(Stack* stack) {
	//while (symbol_stack_pop(stack));
	StackItem *elemPtr;
	while (stack->topPtr != NULL) {		
		elemPtr = stack->topPtr;
		stack->topPtr = stack->topPtr->nextPtr;
		free(elemPtr);
	}
}

void stackPopCount(Stack* stack, int count) {
	int i = 0;
	while (i < count) {
		StackItem *elemPtr;
		if (stack->topPtr != NULL) {
			elemPtr = stack->topPtr;
			stack->topPtr = stack->topPtr->nextPtr;
			free(elemPtr);
		}
		i++;
	}
}

StackItem* stackTopTerminal(Stack* stack) {
	StackItem* elemPtr = stack->topPtr;
	while (elemPtr != NULL) {
		if (elemPtr->symbol < STOP) {
			return elemPtr;
		}
		elemPtr = elemPtr->nextPtr;
	}
	return NULL;
}
/*
bool symbolStackInsertAfterTopTerminal(Stack* stack, precAnalysisTableSymbol symbol, dataTypeEnum type) {
	StackItem* tmp = NULL;
	StackItem* elemPtr = stack->topPtr;
	while (elemPtr != NULL) {
		if (elemPtr->symbol < STOP) {
			StackItem* newElemPtr = (StackItem*)malloc(sizeof(StackItem));
			if (newElemPtr == NULL) {
				return false;
			}
			newElemPtr->symbol = symbol;
			newElemPtr->dataType = type;
			if (tmp == NULL) {
				newElemPtr->nextPtr = stack->topPtr;
				stack->topPtr = newElemPtr;
			} else {
				newElemPtr->nextPtr = tmp->nextPtr;
				tmp->nextPtr = newElemPtr;
			}
			return true;
		}
		tmp = elemPtr;
		elemPtr = elemPtr->nextPtr;
	}
	return false;
}
*/

bool symbolStackInsertAfterTopTerminal(Stack* stack, precAnalysisTableSymbol symbol, dataTypeEnum type)
{
	StackItem* prev = NULL;

	for (StackItem* tmp = stack->topPtr; tmp != NULL; tmp = tmp->nextPtr)
	{
		if (tmp->symbol < STOP)
		{
			StackItem* new_item = (StackItem*)malloc(sizeof(StackItem));

			if (new_item == NULL)
				return false;

			new_item->symbol = symbol;
			new_item->dataType = type;

			if (prev == NULL)
			{
				new_item->nextPtr = stack->topPtr;
				stack->topPtr = new_item;
			}
			else
			{
				new_item->nextPtr = prev->nextPtr;
				prev->nextPtr = new_item;
			}

			return true;
		}

		prev = tmp;
	}

	return false;
}

void printStack(Stack* stack){
	StackItem *item = stack -> topPtr;
	int i = 0;
	while (item != NULL){
		printf(CRED"    Stack"CWHT" %d :%d\n", ++i, item->symbol);
		item = item -> nextPtr;
	}
	return;
}