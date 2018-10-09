typedef enum {

	stateStart, //[0]; beginning of automaton
	stateSpace, //[1]; is a space
	stateEOL, //[2]; end of line
	stateEqual, //[3]; =
	stateGreater, //[4]; >
	stateLess, //[5]; <
	stateExclamation, //[6]; !
	sStringStart, //[7]; "....






} stateList;




//------------------
typedef enum {

	tokenEmpty, //[0], empty token
	tokenAdd, //[1], plus sign
	tokenSub, //[2], minus sign
	tokenMul, //[3], asterisk sign
	tokenDiv, //[4], slash sign
	tokenLeftBracket, //[5] left bracket sign
	tokenRightBracket, //[6] right bracket sign
	tokenComment, //[7] comment (block or single line)
	tokenEmptyString, //[8] "" - empty string

} tokenType;




//-----------------
typedef struct
{
	tType type; /// Token type, one of token_type constants.
	tAttribute attribute; /// Attribute of token.
} Token;


//------------------

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


} keywords; //TODO

void setSourceCodeFile (FILE *f);
void set_dynamic_string(dynamicString *string);

typedef union{
	dynamicString *string; /// String or identifier value.
	int integer; /// Integer value.
	Keyword keyword; /// Keyword, one of the KEYWORD_... constant
	double decimal; /// Decimal value.
} Token_attribute;
