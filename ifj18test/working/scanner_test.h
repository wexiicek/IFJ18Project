#include <stdio.h>
#include "string.h"




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
	tokenInteger,
	tokenString,
	tokenEscapeSequence,
} TokenType;

typedef enum {
	KW_DEF,
	KW_DO,
	KW_ELSE,
	KW_END,
	KW_IF,
	KW_NOT,
	KW_NIL,
	KW_THEN,
	KW_WHILE,
} Keyword;

//Structure that keeps the value of a certain token.
typedef union{
	int integer; /// Integer value.
	string *string;
	Keyword keyword;
} TokenData;

//Structure that defines a certain token.
typedef struct {
	TokenType Type;
	TokenData Data;
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
	stateStringEnd,
	stateNumberEnd,
	stateEscapeSequence,
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

