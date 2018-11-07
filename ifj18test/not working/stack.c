#include <stdlib.h>
#include "stack.h"

void stackInit(Stack* stack) {
	stack->top = NULL;
}

bool stackPush(Stack* stack, Prec_table_symbol_enum symbol, Data_type type) {
	StackItem* newElemPtr = (StackItem *) malloc(sizeof(StackItem));
	if (newElemPtr == NULL) {
		return false;
	}
	newElemPtr->symbol = symbol;
	newElemPtr->data_type = type;
	newElemPtr->nextPtr = stack->topPtr;
	stack->topPtr = newElemPtr;
	return true;
} 

bool stackPop(Stack* stack) {
	StackItem *elemPtr;
	if (stack->topPtr != NULL) {
		elemPtr = stack->topPtr;
		s->topPtr = s->topPtr->nextPtr;
		free(elemPtr);
		return true;
	}
	return false;
}

StackItem* stackTop(Stack *stack) {
	return (s->topPtr->data);
}

void stackFree(Stack* stack) {
	//while (symbol_stack_pop(stack));
	while (stack->topPtr != NULL) {		
		StackItem *elemPtr = stack->topPtr;
		s->topPtr = s->topPtr->nextPtr;
		free(elemPtr);
	}
}

void stackPopCount(Stack* stack, int count) {
	int i = 0;
	while (i < count) {
		StackItem *elemPtr;
		if (stack->topPtr != NULL) {
			elemPtr = stack->topPtr;
			s->topPtr = s->topPtr->nextPtr;
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

bool symbolStackInsertAfterTopTerminal(Stack* stack, Prec_table_symbol_enum symbol, Data_type type) {
	StackItem* tmp = NULL;
	StackItem* elemPtr = stack->topPtr;
	while (elemPtr != NULL) {
		if (elemPtr->symbol < STOP) {
			StackItem* newElemPtr = (StackItem*)malloc(sizeof(StackItem));
			if (newElemPtr == NULL) {
				return false;
			}
			newElemPtr->symbol = symbol;
			newElemPtr->data_type = data_type;
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
