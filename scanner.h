typedef enum {

	stateStart, //[0]; beginning of automaton
	stateEOL, //[2]; end of line
	stateEqual, //[3]; =
	stateGreater, //[4]; >
	stateLess, //[5]; <
	stateExclamation, //[6]; !
	stateStringStart, //[7]; "....
	stateSlash,
	stateBackslash,

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
	tokenBlockComment,
	tokenEmptyString, //[8] "" - empty string
	tokenCompare,
	tokenAssign,
	tokenGreater, //when we get >
	tokenGreaterEqual, //when we get >=
	tokenGreaterThan, //when we get > and not=, means we have comparison
	tokenLess, //when we get <
	tokenLessEqual, //when we get <=
	tokenLessThab, //when we get < and not =, means we have comparison

} tokenType;




//-----------------
typedef struct {

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

} keywords; //TODO

void setSourceCodeFile (FILE *f);
void set_dynamic_string(dynamicString *string);

typedef union {

	dynamicString *string; /// String or identifier value.
	int integer; /// Integer value.
	Keyword keyword; /// Keyword, one of the KEYWORD_... constant
	double decimal; /// Decimal value.

} tokenAttribute;
