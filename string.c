#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "string.h"
#include "err.h"

#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"
int allocCounter = 0;
int stringInit(dynString *str) {
    fprintf(stderr, CRED"%d. ALLOCATING STRING \n"CWHT, ++allocCounter );
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
        fprintf(stderr, CRED"%d. REALLOCATING STRING \n"CWHT, ++allocCounter );

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

bool pushToToken(dynString *from, dynString *to){
    int len = from -> length;
    printf("len%d\n", len);
    if (len >= to -> lengthAllocated ){

        to -> value = (char *) realloc(to -> value, len + 1); 
        to -> lengthAllocated = len +1;
    }

        fprintf(stderr, "%s\n","realloc" );
    strcpy(from -> value, to -> value);
    printf("%s\n", to->value);
    to -> length = len;
    return true;
}
/*
bool strToNumToken(Token token){
    int value = atoi(token->Data.string->value);
    printf("%d\n",value);
    return true;
}*/

bool charIsLowercase(char c){
	if (c >= 'a' && c <= 'z')
		return true;
	return false;
}