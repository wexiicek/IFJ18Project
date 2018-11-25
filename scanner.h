#ifndef SCANNER_HEADER
#define SCANNER_HEADER

#include <stdio.h>

#include "string.h"
typedef enum {
	tokenEmpty,				//0
	tokenAdd, 				//1
	tokenSub,				//2
	tokenMul,				//3
	tokenDiv,				//4
	tokenLeftBracket,		//5
	tokenRightBracket,		//6
	tokenLeftBrace,			//7
	tokenRightBrace,		//8
	tokenComma,				//9
	tokenEndOfFile,			//10
	tokenFloat,				//11
	tokenExponential,		//12
	tokenEqual,				//13
	tokenNotEqual,			//14
	tokenLess,				//15
	tokenGreater,			//16
	tokenLessEqual,			//17
	tokenGreaterEqual,		//18
	tokenEndOfLine,			//19
	tokenNumber,			//20
	tokenAssign,			//21
	tokenInteger,			//22
	tokenString,			//23
	tokenEscapeSequence,	//24
	tokenKeyword,			//25
	tokenIdentifier			//26
} TokenType;

typedef enum {
	KW_CHR,
	KW_DEF,
	KW_DO,
	KW_ELSE,
	KW_END,
	KW_IF,
	KW_INPUTF,
	KW_INPUTI,
	KW_INPUTS,
	KW_LENGTH,
	KW_NOT,
	KW_NIL,
	KW_ORD,
	KW_THEN,
	KW_WHILE,
	KW_PRINT,
	KW_SUBSTR,
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
	stateIdentifierEnd,
	stateNumberEnd,
	stateEscapeSequence,
	stateBlockComment,
	stateError,
} stateList;

int getTokens (Token *token);
int setSourceFile(FILE *sourceFile);
void setDynString(dynString *string);
int checkAndSet(Token *token);
#endif