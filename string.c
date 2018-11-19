#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "string.h"

int stringInit(dynString *str) {
    str->value = (char*) malloc(initAllocSize * sizeof(char)); 
    if(str->value == NULL){
        return 1;
    }
    
    str->length = 0;
    str->value[0] = '\0';
    str->lengthAllocated = initAllocSize;
    
    return 0;
}

bool stringCompare(dynString *str1, char *str2){
    bool tmp;
    tmp = (strcmp(str1->value, str2));
    if (tmp == 0)
        return true;
    else
        return false;
}

int stringAddChar(dynString *str, char c) {
    // If we need more space for additional char,
    //Then we use realloc.
    if (str->length+1 >= str->lengthAllocated) {
        str->value = (char *) realloc(str->value, str->lengthAllocated + initAllocSize * sizeof(char));
        if (str->value == NULL){    
            return 1;
        }
        str->lengthAllocated = str->lengthAllocated + initAllocSize;
    }
    //adding a chat at the end of the string
    str->value[str->length] = c;
    str->value[str->length+1] = '\0';
    str->length++;

    return 0;
}

int stringClear(dynString *str) {
    int i;
    for (i = 0; i < str->lengthAllocated; i++) {
        str->value[i] = '\0';
    }
    str->length = 0;
    
    return 0;
}

void stringDispose(dynString *str) {
    free(str->value);
    //str->value = NULL;
    //str->length = 0;
    //str->lengthAllocated = 0;
}

bool charIsLowercase(char c){
	if (c >= 'a' && c <= 'z')
		return true;
	return false;
}