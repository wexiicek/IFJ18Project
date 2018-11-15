#include "string.h"
typedef struct {
    char *value; // string
    int length; // string length
    int lengthAllocated; // allocated size
} dynString;

dynString kwstring;

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
	tokenKeyword
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

typedef union{
	int integer; 
	dynString *string;
	Keyword keyword;
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

typedef struct{

	Token token;				/// Token

} parserData;

