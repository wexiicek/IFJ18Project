/*
typedef enum {

	AS,
	DECLARE,
	DIM,
	DO,
	DOUBLE,
	ELSE,
	END,
	T_FUNCTION,
	IF,
	INPUT,	
	INTEGER,
	LENGTH,
	SUBSTR,
	ASC,
	CHR,
	LOOP,
	PRINT,
	RETURN,
	SCOPE,
	STRING,	
	THEN,
	WHILE,
	AND,
	BOOLEAN,
	CONTINUE,
	ELSEIF,
	EXIT,
	T_FALSE,
	FOR,
	NEXT,
	NOT,
	OR,
	SHARED,	
	STATIC,
	ADD,
	SUB,
	MUL,
	DIV,
	INT_DIV = 40,
	LESS,
	GREATER,
	LESS_EQ,
	GREATER_EQ,
	ASSIGNMENT_EQ,
	NEQ,
	ID,
	INT_NUMBER,
	DOUBLE_NUMBER,
	TEXT,
	BACKSLASH,
	END_OF_FILE,
	COMMA,
	LEFT_R_BRACKET,
	RIGHT_R_BRACKET,
	SEMICOLON,
	T_TRUE,
	EOL,
	DOT

} tokens_t;*/

typedef enum {
    def,
    do,
    else,
    end,
    if, 
    not,
    nil,
    then,
    while,

}