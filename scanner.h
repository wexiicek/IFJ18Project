/*
typedef enum {


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

} tokens_t;*/

typedef enum {
    
    DEF,
    DO,
    ELSE,
    END,
    IF, 
    NOT,
    NIL,
    THEN,
    WHILE,
    //LIST OF KEYWORDS FROM TASK 3.1
    QUESTIONMARK,   //OTÁZNIK
    EXCLAMATIONMARK, //VÝKRIČNÍK
    LEFTBRACKET,    //LAVÁ ZÁVORKA
    RIGHTBRACKET,   //PRAVÁ ZÁVORKA
    SEMICOLON,  //;
    DOT,    //.
    EOL,    //koniec riadku
    COMMA, //,
    DOUBLEQUOTES, //uvodzovky
    BACKSLASH, //lomitko
    ADD, //+
    SUB, //-
    DIV, // /
    MUL, // *
}