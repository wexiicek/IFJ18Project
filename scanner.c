#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "string.h"
#include "scanner.h"
#include "parser.h"
#include "err.h"

/*
 * Macros for colored output
*/
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"


#define scanRet(string, value)\
			stringDispose(string);\
			return value

#define nextChar(character) character = getc(code)
#define returnChar(character, source) ungetc(character, source)

FILE *code;


unsigned int keywordCompare (dynString *kwstring, Token *token){

	bool kw = false;


	if(stringCompare(kwstring, "chr"))
		{token -> Data.keyword = KW_CHR; kw = true;}
	else if (stringCompare(kwstring, "def"))
		{token->Data.keyword = KW_DEF; kw = true;}
	else if (stringCompare(kwstring, "do"))
		{token->Data.keyword = KW_DO; kw = true;}
	else if (stringCompare(kwstring, "else"))
		{token->Data.keyword = KW_ELSE; kw = true;}
	else if (stringCompare(kwstring, "end"))  
		{token->Data.keyword = KW_END; kw = true;}
	else if (stringCompare(kwstring, "if"))
		{token->Data.keyword = KW_IF; kw = true;}
	else if (stringCompare(kwstring, "inputf"))
		{token->Data.keyword = KW_INPUTF; kw = true;}
	else if (stringCompare(kwstring, "inputi"))
		{token->Data.keyword = KW_INPUTI; kw = true;}
	else if (stringCompare(kwstring, "inputs"))
		{token->Data.keyword = KW_INPUTS; kw = true;}
	else if (stringCompare(kwstring, "length"))
		{token -> Data.keyword = KW_LENGTH; kw = true;}
	else if (stringCompare(kwstring, "not"))
		{token->Data.keyword = KW_NOT; kw = true;}
	else if (stringCompare(kwstring, "nil"))
		{token->Data.keyword = KW_NIL; kw = true;}
	else if (stringCompare(kwstring, "ord"))
		{token->Data.keyword = KW_ORD; kw = true;}
	else if (stringCompare(kwstring, "then"))
		{token->Data.keyword = KW_THEN; kw = true;}
	else if (stringCompare(kwstring, "while"))
		{token->Data.keyword = KW_WHILE; kw = true;}
	else if (stringCompare(kwstring, "print"))
		{token->Data.keyword = KW_PRINT; kw = true;}
	else if (stringCompare(kwstring, "substr"))
		{token->Data.keyword = KW_SUBSTR; kw = true;}
	
	if(kw)
		{fprintf(stderr,"    KEYWORD: %d, ", token->Data.keyword); token->Type = tokenKeyword;}


	return SUCCESS;
}

dynString *kwstring;

int setSourceFile(FILE *sourceFile) {
	if((code = sourceFile) == NULL)
		return SUCCESS;
	return INTERNAL;
}

void setDynString(dynString *string){
	kwstring = string;
}

int checkAndSet(Token *token){
	if(code == NULL || kwstring == NULL)
		return INTERNAL;



	fprintf(stderr,CGRN"[SCANNER]"CWHT" File "CGRN"OK"CWHT", Dynamic String "CGRN"OK\n");
	
	token -> Data.string = kwstring;

	fprintf(stderr,CGRN "[SCANNER]"CWHT" Looking for tokens->\n");
	return SUCCESS;
}

int getTokens (Token *token) {	

	dynString string;
	dynString *str = &string;

	if(stringInit(str))
		return INTERNAL;

	token -> Data.string = kwstring;
	
	bool zeroSwitch = false;
	int state = stateStart; token -> Type = tokenEmpty;
	char c;
	char *prt = "    TOKEN TYPE:";
	

	//Checking for block comment at the beginning, when there is no EOL before.. 
	// .. dont ask
	nextChar(c);
	if ((c == '=')){
		state = stateError;
		nextChar(c);               										
		if (c == 'b'){
			nextChar(c);
			if(c == 'e'){
				nextChar(c);                										
				if(c == 'g'){
					nextChar(c);                										
					if(c == 'i'){
						nextChar(c);                								
						if(c == 'n'){
							nextChar(c);                					
							if (c ==  '\n') {
								while (1){
									nextChar(c);
									if(c == '\n'){
										nextChar(c);
										if(c == '='){
											nextChar(c);
											if( c == 'e'){
												nextChar(c);
												if(c == 'n'){
													nextChar(c);
													if(c == 'd'){
														nextChar(c);
														if(c == '\n' || c == EOF){
															ungetc(c, code);
															state = stateStart;
															break;
	}}}}}}}}}}}}}} else ungetc(c, code);

	//Go through every character of a file
	while (1){
		nextChar(c);
		switch (state){
			case (stateStart):

			/*	Cases, where we CAN determine the type definitely	*/

				if (c == '\n'){
					nextChar(c);
					if ((c == '=')){
						state = stateError;
						nextChar(c);                										
						if (c == 'b'){
							nextChar(c);
							if(c == 'e'){
								nextChar(c);                										
								if(c == 'g'){
									nextChar(c);                										
									if(c == 'i'){
										nextChar(c);                								
										if(c == 'n'){
											nextChar(c);                					
                							if (c ==  '\n') {
                								while (1){
                									nextChar(c);
                									if(c == '\n'){
                										nextChar(c);
                										if(c == '='){
                											nextChar(c);
                											if( c == 'e'){
	                											nextChar(c);
	                											if(c == 'n'){
	                												nextChar(c);
	                												if(c == 'd'){
	                													nextChar(c);
	                													if(c == '\n' || c == EOF){
	                														ungetc(c, code);
	                														state = stateStart;
	                														break;
	                									   }}}}}}}}}}}}}}
                										
												
					else{
						ungetc(c, code);
						token -> Type = tokenEndOfLine;
						state = stateEnd;
						}
				}

				else if (isspace(c))
					{state = stateStart; token->Type = tokenEmpty;}

				else if (c == '+')
					{token->Type = tokenAdd; fprintf(stderr,"%s ADD\n", prt); return SUCCESS;}

				else if (c == '-')
					{token->Type = tokenSub; fprintf(stderr,"%s SUB\n", prt); return SUCCESS;}
				
				else if (c == '*')
					{token->Type = tokenMul; fprintf(stderr,"%s MUL\n", prt); return SUCCESS;}
				
				else if (c == '/') 
					{token->Type = tokenDiv; fprintf(stderr,"%s DIV\n", prt); return SUCCESS;}
				
				else if (c == '(')
					{token->Type = tokenLeftBracket; fprintf(stderr,"%s LBRACK\n", prt); return SUCCESS;}
				
				else if (c == ')')
					{token->Type = tokenRightBracket; fprintf(stderr,"%s RBRACK\n", prt); return SUCCESS;}

				else if (c == '{')
					{token->Type = tokenLeftBrace; fprintf(stderr,"%s LBRACE\n", prt); return SUCCESS;}

				else if (c == '}')
					{token->Type = tokenRightBrace; fprintf(stderr,"%s RBRACE\n", prt); return SUCCESS;}

				else if (c == ',')
					{token->Type = tokenComma; fprintf(stderr,"%s COMMA\n", prt); return SUCCESS;}


			/*	Cases, where we CAN'T determine the type definitely	*/

				else if (c == '"')
					{state = stateStringStart; token->Type = tokenString;}
				
				else if (c == '<')
					state = stateLess;
				
				else if (c == '>')
					state = stateGreater;
				
				else if (c == '=')
					state = stateEqual;

				else if (isdigit(c)){
					ungetc(c, code); state = stateNumber; token->Type = tokenInteger;

				}
				
				else if (c == '#')
					{state = stateComment; token->Type = tokenEmpty;}
				//Question: do we need EOL token on the end of single line
				//			comment?

				else if (c == '!')
					state = stateExclamation;
			
				else if (c == EOF)
					{token->Type = tokenEndOfFile; state = stateEnd;}
				//TODO
			
				else if ( (c == '_') || (charIsLowercase(c)) )
					{ungetc(c, code); state = stateIdentifierOrKeyword; token->Type = tokenIdentifier;}
				
				else 
					{token->Type = tokenEmpty; return LEXICAL;}
			break;

			case (stateNumber):
				if ((zeroSwitch) && (token->Type == tokenInteger) && (isdigit(c)))
					{fprintf(stderr,"%d\n", zeroSwitch); zeroSwitch = false;return LEXICAL; }

				if (isdigit(c)){
					if (c == '0')
						zeroSwitch = true;
					stringAddChar(kwstring, c);
				}

				else if (c == '.')
					{token->Type = tokenFloat; stringAddChar(kwstring, c);}

				else if ((c == 'e')||(c == 'E'))
					{token->Type = tokenExponential; stringAddChar(kwstring, c);}

				else
					{state = stateNumberEnd; zeroSwitch = false; ungetc(c, code); }
			break;

			case (stateIdentifierOrKeyword):
				if ( (c == '_') || isalnum(c))
					{stringAddChar(kwstring, c);}
				else if ( (c == '?') || (c == '!') )
					{stringAddChar(kwstring, c); state = stateIdentifierEnd;}
				else if (  (c == ',')  || (c == '=') || isspace(c) || (c == EOF))
					{ungetc(c, code); state = stateIdentifierEnd;}
				else
					{ungetc(c, code); state = stateIdentifierEnd;}
			break;


			case (stateIdentifierCheck):
				if (isspace(c))
					{ungetc(c, code); state = stateStart;}
			break;

			case (stateEqual):
				if (c == '=')
					{token->Type = tokenEqual; state = stateStart;  fprintf(stderr,"%s EQUAL", prt); return SUCCESS;}
				else if(c == 'b')
					{ungetc(c, code); state = stateBlockComment;}
				else 
					{ungetc(c, code); token->Type = tokenAssign; state = stateStart;}

			break;

			case (stateLess):
				if (c == '=')
					{token->Type = tokenLessEqual; state = stateStart;  fprintf(stderr,"%s LESS EQUAL", prt); return SUCCESS;}
				else
					{ungetc(c, code); token->Type = tokenLess; state = stateStart;}
			break;

			case (stateGreater):
				if (c == '=')
					{token->Type = tokenGreaterEqual; state = stateStart;  fprintf(stderr,"%s GREATER EQUAL", prt); return SUCCESS;}
				else
					{ungetc(c, code); token->Type = tokenGreater; state = stateStart;}
			break;

			case (stateExclamation):
				if (c == '=')
					{token->Type = tokenNotEqual; state = stateStart;  fprintf(stderr,"%s NOT EQUAL", prt); return SUCCESS;}
				else
					return LEXICAL;
			break;

			case (stateComment):
				if (c == '\n')
					{state = stateStart;}
				if (c == EOF)
					{token->Type = tokenEndOfFile; state = stateEnd;}
				else
			//		continue;
			break;

			case (stateBlockComment):

			break;

			case (stateStringStart):
				token->Type = tokenString;

				if (c < 32)
					{scanRet(kwstring, LEXICAL);}

				else if (c == '\\')
					{state = stateEscapeSequence;}

				else if (c != '\"')
					stringAddChar(kwstring, c);
				else 
					{state = stateStringEnd;}
			break;

			case (stateEscapeSequence):


				if (token->Type == 24)
					fprintf(stderr,"    TOKEN TYPE: ESCAPE\n");
				if (c == 'n'){
					c = '\n';
					token->Type = tokenEscapeSequence;
					stringAddChar(kwstring, c);
					state = stateStringStart;
				}

				else if (c == 't'){
					c = '\t';
					stringAddChar(kwstring, c);
					state = stateStringStart;
				}

				else if (c == 's'){
					c = ' ';
					stringAddChar(kwstring, c);
					state = stateStringStart;
				}

				else if (c == '"'){
					c = '"';
					stringAddChar(kwstring, c);
					state = stateStringStart;
				}

			break;

			case (stateStringEnd):
				if(!stringCompare(kwstring,"\n") == 0)
					fprintf(stderr,"    TOKEN TYPE: STRING | TOKEN VAL: EOL\n");
				else
					fprintf(stderr,"    TOKEN TYPE: %d | TOKEN VAL: \"%s\"\n",token->Type ,kwstring->value);
				state = stateStart;
				stringClear(kwstring);
				ungetc(c, code);
				scanRet(kwstring, LEXICAL);
			break;

			case (stateIdentifierEnd):
			keywordCompare(kwstring, token);
				if(!stringCompare(kwstring,"\n") == 0)
					fprintf(stderr,"    TOKEN TYPE: ID | TOKEN VAL: EOL\n");
				else
					fprintf(stderr,"    TOKEN TYPE: %d | TOKEN VAL: \"%s\"\n",token->Type ,kwstring->value);
				state = stateStart;
				stringClear(kwstring);
				ungetc(c, code);
				//scanRet(kwstring, LEXICAL);
				return SUCCESS;
			break;

			case (stateNumberEnd):
				if (token->Type == 11)
					fprintf(stderr,"    TOKEN TYPE: FLOAT | TOKEN VAL: ");
				else if (token->Type == 12)
					fprintf(stderr,"    TOKEN TYPE: INT | TOKEN VAL: ");
				else if (token->Type == 22)
					fprintf(stderr,"    TOKEN TYPE: INT | TOKEN VAL: ");
				fprintf(stderr,"\"%s\"\n", kwstring->value);
				state = stateStart;
				stringClear(kwstring);
				ungetc(c, code);
				return SUCCESS;

			break;

			case (stateError):
				scanRet(kwstring, LEXICAL);
				break;

			case (stateEnd):
				return SUCCESS;
			break;	
	}
	
	char * tType = "    TOKEN TYPE:";
	switch(token->Type){
		
		case 10:
			fprintf(stderr,"%s EOF\n", tType);
			return SUCCESS;
			break;
		case 12:
			fprintf(stderr,"%s EXPONENTIAL\n", tType);
			return SUCCESS;
			break;
		case 13:
			fprintf(stderr,"%s EQUAL\n", tType);
			return SUCCESS;
			break;
		case 14:
			fprintf(stderr,"%s NOT EQUAL\n", tType);
			return SUCCESS;
			break;
		case 15:
			fprintf(stderr,"%s LESS\n", tType);
			return SUCCESS;
			break;
		case 16:
			fprintf(stderr,"%s GREATER\n", tType);
			return SUCCESS;
			break;
		case 17:
			fprintf(stderr,"%s LESS EQUAL\n", tType);
			return SUCCESS;
			break;
		case 18:
			fprintf(stderr,"%s GREATER EQUAL\n", tType);
			return SUCCESS;
			break;
		case 19:
			fprintf(stderr,"%s EOL\n", tType);
			return SUCCESS;
			break;
		case 20:
			fprintf(stderr,"%s NUMBER\n", tType);
			return SUCCESS;
			break;
		case 21:
			fprintf(stderr,"%s ASSIGN\n", tType);
			return SUCCESS;
			break;
		default:
			break;
		}
	}
}