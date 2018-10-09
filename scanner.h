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
typedef union
{
	Dynamic_string *string; /// String or identifier value.
	int integer; /// Integer value.
	Keyword keyword; /// Keyword, one of the KEYWORD_... constant
	double decimal; /// Decimal value.
} Token_attribute;

/**
 * @struct Token representation.
 */
typedef struct
{
	Token_type type; /// Token type, one of token_type constants.
	Token_attribute attribute; /// Attribute of token.
} Token;