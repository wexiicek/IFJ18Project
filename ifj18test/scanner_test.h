#include <stdio.h>
//Structure that keeps the value of a certain token.
typedef union{
	int integer; /// Integer value.
	char *string;
} tokenData;

//Structure that keeps the type of a certain token.
typedef enum {
	tokenEmpty,
	tokenAdd,
	tokenSub,
	tokenMul,
	tokenDiv,
	tokenLeftBracket,
	tokenRightBracket,
	tokenLeftBrace,
	tokenRightBrace,
	tokenComma,
	tokenEndOfFile,
	tokenFloat,
	tokenExponential,
	tokenEqual,
	tokenNotEqual,
	tokenLess,
	tokenGreater,
	tokenLessEqual,
	tokenGreaterEqual,
	tokenEndOfLine,
	tokenNumber,
	tokenAssign,
} tokenType;

//Structure that defines a certain token.
typedef struct {
	tokenType Type;
	tokenData Data;
} Token;

//List of states for finite automaton of scanner (lexer)
typedef enum {
	stateStart,
	stateStringStart,
	stateLess,
	stateGreater,
	stateEqual,
	stateComment,
	stateExclamation,
	stateNumber,
	stateIdentifierCheck,
	stateIdentifierOrKeyword,
	stateZero,
	stateEnd,
	stateEOL,
} stateList;

//List of error states that could occur during the compiling process
enum errorMessages {
	SUCCESS = 0,
	LEXICAL = 1,
	SYNTACTICAL = 2,
	SEMANTICAL = 3,
	COMPATIBILITY = 4,
	OTHER = 6,
    ZERO_DIV = 9,
	INTERNAL = 99,
};

Token token;

/* @brief
*/
int getTokens (FILE* sourceCode);

