/*/////////////////////////////////////
/                                     /                           
/    Subject: IFJ                     /                           
/    Task: IFJ Project                /                           
/                                     /                           
/          Author (leader)            /      
/           Marian Pukancik           /                        
/              xpukan01               /                     
/                                     /                           
/    Team: 045, Variant I             /                           
/                                     /                           
/////////////////////////////////////*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "string.h"
#include "err.h"



int stringInit(dynString *str) {
    str->value = (char*) malloc(initAllocSize * sizeof(char));
    //If allocation was not successfull 
    if(str->value == NULL){
        return INTERNAL;
    }
    //Otherwise we set default values
    str->length = 0;
    str->value[0] = '\0';
    str->lengthAllocated = initAllocSize;
    
    return SUCCESS;
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
        str->value = (char *) realloc(str->value, (str->lengthAllocated + initAllocSize * sizeof(char)));

        if (str->value == NULL){    
            return INTERNAL;
        }
        str->lengthAllocated = str->lengthAllocated + initAllocSize;
    }
    //adding char at the end of the string
    str->value[str->length] = c;
    str->value[str->length+1] = '\0';
    str->length++;

    return SUCCESS;
}

int stringAddString(dynString *str, char *input){
    int len = strlen(input);
    int requiredLength = str->length + len + 1;
    if (requiredLength >= str->lengthAllocated){
        if(!(str->value = (char *) realloc(str->value, requiredLength)))
            return INTERNAL;
        str->lengthAllocated = requiredLength;
    }
    str -> length += len;
    strcat(str->value, input);
    str->value[str->length] = '\0';
    return SUCCESS;
}

int stringClear(dynString *str) {
    int i;
    for (i = 0; i < str->lengthAllocated; i++) {
        str->value[i] = '\0';
    }
    str->length = 0;
    
    return SUCCESS;
}

void stringDispose(dynString *str) {
    free(str->value);
}

bool pushToToken(dynString *from, dynString *to){
    int len = from -> length + 1;
    //We have to check if we have enough memory
    if (len >= to -> lengthAllocated ){
        to -> value = (char *) realloc(to -> value, len); 
        to -> lengthAllocated = len;
    }
    strcpy(to -> value, from -> value);
    to -> length = len - 1;
    return true;
}

bool charIsLowercase(char c){
	if (c >= 'a' && c <= 'z')
		return true;
	return false;
}