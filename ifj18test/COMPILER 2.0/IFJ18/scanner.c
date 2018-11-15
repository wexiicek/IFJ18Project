#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "string.h"
#include "scanner.h"
#include "parser.h"


#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"

FILE *code;


unsigned int keywordCompare (dynString *kwstring, Token *token){

	bool kw = false;

	if (stringCompare(kwstring, "def"))
		{token->Data.keyword = KW_DEF; token->Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "do"))
		{token->Data.keyword = KW_DO;token->Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "else"))
		{token->Data.keyword = KW_ELSE;token->Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "end"))  
		{token->Data.keyword = KW_END;token->Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "if"))
		{token->Data.keyword = KW_IF;token->Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "not"))
		{token->Data.keyword = KW_NOT;token->Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "nil"))
		{token->Data.keyword = KW_NIL;token->Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "then"))
		{token->Data.keyword = KW_THEN;token->Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "while"))
		{token->Data.keyword = KW_WHILE;token->Type = tokenKeyword; kw = true;}

	if(kw)
		{printf("    KEYWORD: %d, ", token->Data.keyword); token->Type = tokenKeyword;}


	return 0;
}

dynString *kwstring;

int setSourceFile(FILE *sourceFile) {
	if((code = sourceFile) == NULL)
		return 0;
	return 1;
}

void setDynString(dynString *string){
	kwstring = string;
}

int checkAndSet(Token *token){
	if(code == NULL || kwstring == NULL)
		return 0;



	printf(CGRN"[SCANNER] File "CGRN"OK"CWHT", Dynamic String "CGRN"OK\n");
	token -> Data.string = kwstring;

	printf(CGRN "[SCANNER]"CWHT" Looking for tokens->\n");
	return 1;
}

int getTokens (Token *token) {
	

	dynString string;
	dynString *str = &string;

	if(stringInit(str))
		return 1;




	
	bool zeroSwitch = false;
	int state = stateStart; token -> Type = tokenEmpty;
	char c;
	//Go through every character of a file
	while (1){
		c = getc(code);
		switch (state){
			case (stateStart):

			/*	Cases, where we CAN determine the type definitely	*/

				if (c == '\n')
					{token->Type = tokenEndOfLine;}

				else if (isspace(c))
					{state = stateStart; token->Type = tokenEmpty;}

				else if (c == '+')
					{token->Type = tokenAdd;}

				else if (c == '-')
					{token->Type = tokenSub;}
				
				else if (c == '*')
					{token->Type = tokenMul;}
				
				else if (c == '/') 
					{token->Type = tokenDiv;}
				
				else if (c == '(')
					{token->Type = tokenLeftBracket;}
				
				else if (c == ')')
					{token->Type = tokenRightBracket;}

				else if (c == '{')
					{token->Type = tokenLeftBrace;}

				else if (c == '}')
					{token->Type = tokenRightBrace;}

				else if (c == ',')
					{token->Type = tokenComma;}


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
					{token->Type = tokenEmpty; return 2;}
			break;

			case (stateNumber):
				if ((zeroSwitch) && (token->Type == tokenInteger) && (isdigit(c)))
					{printf("%d\n", zeroSwitch); zeroSwitch = false;return 2; }

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
					{stringAddChar(kwstring, c); state = stateStringEnd;}
				else if (  (c == ',')  || (c == '=') || isspace(c) || (c == EOF))
					{ungetc(c, code); state = stateStringEnd;}
				else
					{ungetc(c, code); state = stateStringEnd;}
			break;


			case (stateIdentifierCheck):
				if (isspace(c))
					{ungetc(c, code); state = stateStart;}
			break;

			case (stateEqual):
				if (c == '=')
					{token->Type = tokenEqual; state = stateStart;}
				else 
					{ungetc(c, code); token->Type = tokenAssign; state = stateStart;}

			break;

			case (stateLess):
				if (c == '=')
					{token->Type = tokenLessEqual; state = stateStart;}
				else
					{ungetc(c, code); token->Type = tokenLess; state = stateStart;}
			break;

			case (stateGreater):
				if (c == '=')
					{token->Type = tokenGreaterEqual; state = stateStart;}
				else
					{ungetc(c, code); token->Type = tokenGreater; state = stateStart;}
			break;

			case (stateExclamation):
				if (c == '=')
					{token->Type = tokenNotEqual; state = stateStart;}
				else
					return 2;
			break;

			case (stateComment):
				if (c == '\n')
					{ungetc(c, code); state = stateStart;}
				if (c == EOF)
					{token->Type = tokenEndOfFile; state = stateEnd;}
				else
			//		continue;
			break;

			case (stateStringStart):
				token->Type = tokenString;

				if (c < 32)
					return 2;

				else if (c == '\\')
					{state = stateEscapeSequence;}

				else if (c != '\"')
					stringAddChar(kwstring, c);
				else 
					{state = stateStringEnd;}
			break;

			case (stateEscapeSequence):


				if (token->Type == 24)
					printf("    TOKEN TYPE: ESCAPE\n");
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
			keywordCompare(kwstring, token);
				if(!stringCompare(kwstring,"\n") == 0)
					printf("    TOKEN TYPE: STRING | TOKEN VAL: EOL\n");
				else
					printf("    TOKEN TYPE: %d | TOKEN VAL: \"%s\"\n",token->Type ,kwstring->value);
				state = stateStart;
				stringClear(kwstring);
				ungetc(c, code);
				stringDispose(kwstring);
				return 1;
			break;

			case (stateNumberEnd):
				if (token->Type == 11)
					printf("    TOKEN TYPE: FLOAT | TOKEN VAL: ");
				else if (token->Type == 12)
					printf("    TOKEN TYPE: INT | TOKEN VAL: \n");
				else if (token->Type == 22)
					printf("    TOKEN TYPE: INT | TOKEN VAL: ");
				printf("\"%s\"\n", kwstring->value);
				state = stateStart;
				stringClear(kwstring);
				ungetc(c, code);
				stringDispose(kwstring);
			break;

			case (stateEnd):
				return 0;
			break;	
	}

		FILE *log = fopen("log->txt", "a");
		fprintf(log, "%c\n", c);		
		fclose(log);

	
	char * tType = "    TOKEN TYPE:";
	switch(token->Type){
		case 1:
			printf("%s ADD\n", tType);
			return 0;
			break;
		case 2:
			printf("%s SUB\n", tType);
			return 0;
			break;
		case 3:
			printf("%s MUL\n", tType);
			return 0;
			break;
		case 4:
			printf("%s DIV\n", tType);
			return 0;
			break;
		case 5:
			printf("%s L BRACKET\n", tType);
			return 0;
			break;
		case 6:
			printf("%s R BRACKET\n", tType);
			return 0;
			break;
		case 7:
			printf("%s L BRACE\n", tType);
			return 0;
			break;
		case 8:
			printf("%s R BRACE\n", tType);
			return 0;
			break;
		case 9:
			printf("%s COMMA\n", tType);
			return 0;
			break;
		case 10:
			printf("%s EOF\n", tType);
			return 0;
			break;
		case 12:
			printf("%s EXPONENTIAL\n", tType);
			return 0;
			break;
		case 13:
			printf("%s EQUAL\n", tType);
			return 0;
			break;
		case 14:
			printf("%s NOT EQUAL\n", tType);
			return 0;
			break;
		case 15:
			printf("%s LESS\n", tType);
			return 0;
			break;
		case 16:
			printf("%s GREATER\n", tType);
			return 0;
			break;
		case 17:
			printf("%s LESS EQUAL\n", tType);
			return 0;
			break;
		case 18:
			printf("%s GREATER EQUAL\n", tType);
			return 0;
			break;
		case 19:
			printf("%s EOL\n", tType);
			return 0;
			break;
		case 20:
			printf("%s NUMBER\n", tType);
			return 0;
			break;
		case 21:
			printf("%s ASSIGN\n", tType);
			return 0;
			break;
		default:
			break;
		}
	}
}