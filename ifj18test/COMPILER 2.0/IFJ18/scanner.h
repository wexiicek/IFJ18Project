#ifndef _SCANNER_H
#define _SCANNER_H

#include <stdio.h>

#include "string.h"
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
	tokenKeyword,
	tokenIdentifier
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
	KW_PRINT,
} Keyword;

typedef union{
	int integer; 
	dynString *string;
	Keyword keyword;
	double flt; // float
} TokenData;

typedef struct {
	TokenType Type;
	TokenData Data;
} Token;

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


int getTokens (Token *token);
int setSourceFile(FILE *sourceFile);
void setDynString(dynString *string);
int checkAndSet(Token *token);
#endif