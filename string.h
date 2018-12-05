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

#ifndef STRING_HEADER
#define STRING_HEADER

#include <stdbool.h>


#define initAllocSize 10

typedef struct {
    char *value;         // string
    int length;          // string length
    int lengthAllocated; // allocated size
} dynString;

/**
 * Function is allocating memory setting default values of the dynamic string
 * 
 * @param str    -  Pointer to given string
 * @return       -  Given exit code
 */
int stringInit(dynString *str);

/**
 * Function compares two strings
 * 
 * @param a      -  Pointer to first string
 * @param b      -  Pointer to second string
 * @result       -  Bool value.
 */
bool stringCompare(dynString *a, char *b);

/**
 * Function adds char to a dynamic string
 * 
 * @param str    -  Pointer to dynamic string
 * @param c      -  Pointer to a char which we want to add
 * @result       -  Given exit code
 */
int stringAddChar(dynString *, char);

/**
 * Function adds string into dynamic string
 * 
 * @param str    -  Pointer to dynamic string
 * @param input  -  Pointer to a string which we want to add
 * @result       -  Given exit code
 */
int stringAddString(dynString *, char *);

/**
 * Function clears the whole dynamic string
 * 
 * @param str    -  Pointer to dynamic string
 * @result       -  Given exit code
 */
int stringClear(dynString *);

/**
 * Function disposes the whole dynamic string
 * 
 * @param str    -  Pointer to dynamic string
 */
void stringDispose(dynString *str);

/**
 * Function copies dynamic string of one token to another
 * 
 * @param from   -  Pointer to dynamic string which we want to copy
 * @param to     -  Pointre to dynamic string to which we want to copy
 * @result       -  True if the operation was successfull
 */
bool pushToToken(dynString *from, dynString *to);

/**
 * Function finds out if char is lower case or not
 * 
 * @result       -  Bool value
 */
bool charIsLowercase(char c);


#endif