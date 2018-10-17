#ifndef IFJ_STRING_H
#define IFJ_STRING_H

#define initAllocSize 10   //BC the biggest data type is long double with the size of 10 bytes
                           // Technicaly we need just 4, because we have defined only int, float and string
                           //TODO 

#include <stdbool.h>
//These values are for additional info for other functions in this library 
typedef struct
{
	char *str; /// string ened by '\0' byte
	unsigned int length; /// lenght of string
	unsigned int alloc_size; /// number of chars alocated for string
} Dynamic_string;


int stringLength(Dynamic_string *);
int stringInit(Dynamic_string *);
int stringAddChar(Dynamic_string *, char); //If we want to add more chars at once, then we call this function multiple times
void stringDeleteLastChar(Dynamic_string *);
int stringClear(Dynamic_string *);
bool stringCompare(Dynamic_string *a, Dynamic_string*b);
void stringDispose(Dynamic_string *str);

//void stringUpdateLastChar(string *, char);    Maybe we will need those as well,
//int stringAddFirstChar(string *, char);       if yes, then we will add them as well
//char stringGetCharOnIndex(string *, int);
//void stringUpdateCharOnIndex(string *str, char c, int index);
//void stringToLowercase(string *);

/* ----------------------------------------FUNCTIONS FOR SCANNER-------------------------------------------------*/
//TODO We will need these for sure, but since we cannot work with keywords yet, we will add them later.
//int stringIsKeyWord(string *);
//int stringIsResKeyWord(string *);
//char stringGetLastChar(string *);

/* ------------------------------------FUNCTIONS WORKING WITH CHARS------------------------------------------*/
void charUndo(char);
bool charIsTab(char);
bool charIsSpace(char);
bool charIsBackslash(char);
bool charIsDigit(char);
bool charIsLowercase(char c);
bool charIsLetter(char);
int charToDec(char);
char decToChar(int);

#endif //IFJ_STRING_H