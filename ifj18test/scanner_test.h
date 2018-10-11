#include <stdio.h>
typedef union
{
	int integer; /// Integer value.
	char *string;
} tokenData;

typedef enum {
	tokenAdd,
	tokenSub,
	tokenMul,
	tokenDiv,
	tokenLeftBracket,
	tokenRightBracket,
	tokenWhitespace,
} tokenType;

typedef struct {
	tokenType Type;
	tokenData Data;
} Token;




typedef enum {
	stateStart,
	stateString,

} stateList;

enum errorMessages{
	LEXICAL = 1,
	SYNTACTICAL = 2,
	SEMANTICAL = 3,
	COMPATIBILITY = 4,
	OTHER = 6,
    ZERO_DIV = 9,
	INTERNAL = 99,
};

Token token;

void getTokens (FILE* sourceCode);

