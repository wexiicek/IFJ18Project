#ifndef STRING_HEADER
#define STRING_HEADER

#include <stdbool.h>

#define initAllocSize 10

typedef struct {
    char *value; // string
    int length; // string length
    int lengthAllocated; // allocated size
} dynString;


int stringInit(dynString *str);
int stringAddChar(dynString *, char);
int stringClear(dynString *);
bool stringCompare(dynString *a, char *b);
void stringDispose(dynString *str);
bool charIsLowercase(char c);

#endif