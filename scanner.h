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

    QUESTIONMARK,   //?
    EXCLAMATIONMARK, //!
    COMMA, //,
    DOT,    //.
    LEFTBRACKET,    //LAVÁ ZÁVORKA
    RIGHTBRACKET,   //PRAVÁ ZÁVORKA
    SEMICOLON,  //;
    EOL,    //koniec riadku
    DOUBLEQUOTES, //"
    BACKSLASH, //"\"
    DOUBLEBACKSLASH,
    BACKSLASHX,
    BACKSLASHN,
    BACKSLASHT,
    BACKSLASHS,
    BACKSLASHQUOTES,
    ADD, //+
    SUB, //-
    DIV, // /
    MUL, // *


    POUND, //#
    CBEGIN, //=begin
    CEND, //=end
    GREATER, // >
    LESS, //<
    EQUAL, // ==
    GREATEREQ, // >=
    LESSEQ, //<=
    NOTEQUAL, //!=


} //TODO
>>>>>>> b727ef5a1d79bc1a6d59e8718057b12e8f9ed4e0
