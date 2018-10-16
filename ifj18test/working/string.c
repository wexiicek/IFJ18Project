#include <stdlib.h>
#include <string.h>

#include "string.h"
#include "scanner_test.h"//For errors TODO

int stringLenght(string *str){
    return strlen(str->value);
}

int stringInit(string *str) {
    str->value = (char*) malloc(initAllocSize * sizeof(char)); 
    if(str->value == NULL){
        return INTERNAL;
    }
    
    str->length = 0;
    str->value[0] = '\0';
    str->lengthAllocated = initAllocSize;
    
    return SUCCESS;
}

int stringAddChar(string *str, char c) {
    // If we need more space for additional char,
    //Then we use realloc.
    if (str->length+1 >= str->lengthAllocated) {
        str->value = (char *) realloc(str->value, str->lengthAllocated + initAllocSize * sizeof(char));
        if (str->value == NULL){    
            return INTERNAL;
        }
        str->lengthAllocated = str->lengthAllocated + initAllocSize;
    }
    //adding a chat at the end of the string
    str->value[str->length] = c;
    str->value[str->length+1] = '\0';
    str->length++;

    return SUCCESS;
}

void stringDeleteLastChar(string *str) {
    str->value[str->length-1] = '\0'; //Just rewrites the value of the last char.
    str->length = str->length - 1;
}

int stringClear(string *str) {
    int i;
    for (i = 0; i < str->lengthAllocated; i++) {
        str->value[i] = '\0';
    }
    str->length = 0;
    
    return SUCCESS;
}

bool stringCompare(string *str1, string *str2){
    bool tmp;
    tmp = (strcmp(str1->value, str2->value));
    if (tmp == 0){
        return true;
    }
    else
        return false;
}

void stringDispose(string *str) {
    free(str->value);
    str->value = NULL;
    str->length = 0;
    str->lengthAllocated = 0;
}

/* ------------------------------------FUNCTIONS WORKING WITH CHARS------------------------------------------*/
//Returns char into stdin
void charUndo(char c) {
    ungetc(c, stdin);
}

bool charIsTab(char c) {
    if ( c == '\t' )
        return true;
    else
        return false;
}

bool charIsSpace(char c) {
    if (c == ' ')
        return true;
    else
        return false;
}

bool charIsBackslash(char c) {
    if (c == '\\')
        return true;
    else
        return false;
}

bool charIsDigit(char c) {
    if (c >= '0' && c <= '9')
        return true;
    else
        return false;
}

bool charIsLowercase(char c){
	if (c >= 'a' && c <= 'z')
		return true;
	return false;
}

bool charIsLetter(char c) {
    if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
        return true;
    else
        return false;
}

int charToDec(char c) {
    return c-48;
}

char decToChar(int c) {
    return c+48;
}