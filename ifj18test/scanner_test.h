#include <stdio.h>
typedef struct {
	char Value;
	char Type[50];
} Tokens;

enum errorMessages{
	LEXICAL = 1,
	SYNTACTICAL = 2,
	SEMANTICAL = 3,
	COMPATIBILITY = 4,
	OTHER = 6,
    ZERO_DIV = 9,
	INTERNAL = 99,
};

void getTokens (FILE* sourceCode);

