typedef enum {

	stateStart, //[0]; beginning of finite automaton
	stateEndOfLine, //[2]; end of line
	stateEqual, //[3]; =
	stateGreater, //[4]; >
	stateLess, //[5]; <
	stateExclamation, //[6]; !
	stateStringStart, //[7]; "....
	stateStringContinue,
	stateSlash,
	stateBackslash,
	stateLineComment,
	stateBlockComment,
	stateIdentifierOrKeyword,

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
	tokenEmptyString, //[8] "" - empty string
	tokenUnemptyString,
	tokenCompare,
	tokenAssign,
	tokenEqual,
	tokenNotEqual,
	tokenGreater, //when we get >
	tokenGreaterEqual, //when we get >=
	tokenGreaterThan, //when we get > and not=, means we have comparison
	tokenLess, //when we get <
	tokenLessEqual, //when we get <=
	tokenLessThan, //when we get < and not =, means we have comparison
	tokenEndOfFile,
	tokenEndOfLine,


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
//void setDynamicString(dynamicString *string);

typedef union {

	dynamicString *string; /// String or identifier value.
	int integer; /// Integer value.
	Keyword keyword; /// Keyword, one of the KEYWORD_... constant
	double decimal; /// Decimal value.

} tokenAttribute;
