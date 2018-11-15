#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "scanner_test.h"
#include "string.h"


unsigned int keywordCompare (string *kwstring, Token *token){

	bool kw = false;

	if (stringCompare(kwstring, "def"))
		{token->Data.keyword = KW_DEF; token -> Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "do"))
		{token->Data.keyword = KW_DO;token -> Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "else"))
		{token->Data.keyword = KW_ELSE;token -> Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "end"))
		{token->Data.keyword = KW_END;token -> Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "if"))
		{token->Data.keyword = KW_IF;token -> Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "not"))
		{token->Data.keyword = KW_NOT;token -> Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "nil"))
		{token->Data.keyword = KW_NIL;token -> Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "then"))
		{token->Data.keyword = KW_THEN;token -> Type = tokenKeyword; kw = true;}
	else if (stringCompare(kwstring, "while"))
		{token->Data.keyword = KW_WHILE;token -> Type = tokenKeyword; kw = true;}

	if(kw)
		printf("KEYWORD: %d, ", token->Data.keyword);

	return 0;
}




int getTokens (FILE* sourceCode) {
	if (!sourceCode)
		return INTERNAL;

	printf("hledam tokeny\n");



	
	bool zeroSwitch = false;
	int state = stateStart; token.Type = tokenEmpty;
	char c;
	//Go through every character of a file
	while (1){
		c = getc(sourceCode);
		switch (state){
			case (stateStart):

			/*	Cases, where we CAN determine the type definitely	*/

				if (c == '\n')
					{token.Type = tokenEndOfLine;}

				else if (isspace(c))
					{state = stateStart; token.Type = tokenEmpty;}

				else if (c == '+')
					{token.Type = tokenAdd;}

				else if (c == '-')
					{token.Type = tokenSub;}
				
				else if (c == '*')
					{token.Type = tokenMul;}
				
				else if (c == '/') 
					{token.Type = tokenDiv;}
				
				else if (c == '(')
					{token.Type = tokenLeftBracket;}
				
				else if (c == ')')
					{token.Type = tokenRightBracket;}

				else if (c == '{')
					{token.Type = tokenLeftBrace;}

				else if (c == '}')
					{token.Type = tokenRightBrace;}

				else if (c == ',')
					{token.Type = tokenComma;}


			/*	Cases, where we CAN'T determine the type definitely	*/

				else if (c == '"')
					{state = stateStringStart; token.Type = tokenString;}
				
				else if (c == '<')
					state = stateLess;
				
				else if (c == '>')
					state = stateGreater;
				
				else if (c == '=')
					state = stateEqual;

				else if (isdigit(c)){
					ungetc(c, sourceCode); state = stateNumber; token.Type = tokenInteger;

				}
				
				else if (c == '#')
					{state = stateComment; token.Type = tokenEmpty;}
				//Question: do we need EOL token on the end of single line
				//			comment?

				else if (c == '!')
					state = stateExclamation;
			
				else if (c == EOF)
					{token.Type = tokenEndOfFile; state = stateEnd;}
				//TODO
			
				else if ( (c == '_') || (charIsLowercase(c)) )
					{ungetc(c, sourceCode); state = stateIdentifierOrKeyword; token.Type = tokenString;}
				
				else 
					{token.Type = tokenEmpty; return LEXICAL;}
			break;

			case (stateNumber):
				if ((zeroSwitch) && (token.Type == tokenInteger) && (isdigit(c)))
					{printf("%d\n", zeroSwitch); zeroSwitch = false;return LEXICAL; }

				if (isdigit(c)){
					if (c == '0')
						zeroSwitch = true;
					stringAddChar(&kwstring, c);
				}

				else if (c == '.')
					{token.Type = tokenFloat; stringAddChar(&kwstring, c);}

				else if ((c == 'e')||(c == 'E'))
					{token.Type = tokenExponential; stringAddChar(&kwstring, c);}

				else
					{state = stateNumberEnd; zeroSwitch = false; ungetc(c, sourceCode); }
			break;

			case (stateIdentifierOrKeyword):
				if ( (c == '_') || isalnum(c))
					{stringAddChar(&kwstring, c);}
				else if ( (c == '?') || (c == '!') )
					{stringAddChar(&kwstring, c); state = stateStringEnd;}
				else if (  (c == ',')  || (c == '=') || isspace(c) || (c == EOF))
					{ungetc(c, sourceCode); state = stateStringEnd;}
				else
					{ungetc(c, sourceCode); state = stateStringEnd;}
			break;


			case (stateIdentifierCheck):
				if (isspace(c))
					{ungetc(c, sourceCode); state = stateStart;}
			break;

			case (stateEqual):
				if (c == '=')
					{token.Type = tokenEqual; state = stateStart;}
				else 
					{ungetc(c, sourceCode); token.Type = tokenAssign; state = stateStart;}

			break;

			case (stateLess):
				if (c == '=')
					{token.Type = tokenLessEqual; state = stateStart;}
				else
					{ungetc(c, sourceCode); token.Type = tokenLess; state = stateStart;}
			break;

			case (stateGreater):
				if (c == '=')
					{token.Type = tokenGreaterEqual; state = stateStart;}
				else
					{ungetc(c, sourceCode); token.Type = tokenGreater; state = stateStart;}
			break;

			case (stateExclamation):
				if (c == '=')
					{token.Type = tokenNotEqual; state = stateStart;}
				else
					return LEXICAL;
			break;

			case (stateComment):
				if (c == '\n')
					{ungetc(c, sourceCode); state = stateStart;}
				if (c == EOF)
					{token.Type = tokenEndOfFile; state = stateEnd;}
				else
					continue;
			break;

			case (stateStringStart):
				token.Type = tokenString;

				if (c < 32)
					return LEXICAL;

				else if (c == '\\')
					{state = stateEscapeSequence;}

				else if (c != '\"')
					stringAddChar(&kwstring, c);
				else 
					{state = stateStringEnd;}
			break;

			case (stateEscapeSequence):


				if (token.Type == 24)
					printf("TOKEN TYPE: ESCAPE\n");
				if (c == 'n'){
					c = '\n';
					token.Type = tokenEscapeSequence;
					stringAddChar(&kwstring, c);
					state = stateStringStart;
				}

				else if (c == 't'){
					c = '\t';
					stringAddChar(&kwstring, c);
					state = stateStringStart;
				}

				else if (c == 's'){
					c = ' ';
					stringAddChar(&kwstring, c);
					state = stateStringStart;
				}

				else if (c == '"'){
					c = '"';
					stringAddChar(&kwstring, c);
					state = stateStringStart;
				}

				//else if (c == 'x')

			break;

			case (stateStringEnd):
			keywordCompare(&kwstring, &token);
				if(&kwstring.value == "\n")
					printf("TOKEN TYPE: STRING | TOKEN VAL: EOL\n");
				else
					printf("TOKEN TYPE: STRING | TOKEN VAL: \"%s\"\n", kwstring.value);
				state = stateStart;
				stringClear(&kwstring);
				ungetc(c, sourceCode);
				stringDispose(&kwstring);
			break;

			case (stateNumberEnd):
				if (token.Type == 11)
					printf("TOKEN TYPE: FLOAT\n");
				else if (token.Type == 12)
					printf("TOKEN TYPE: INT\n");
				else if (token.Type == 22)
					printf("TOKEN TYPE: INT ");
				printf("\"%s\"\n", kwstring.value);
				state = stateStart;
				stringClear(&kwstring);
				ungetc(c, sourceCode);
				stringDispose(&kwstring);
			break;

			case (stateEnd):
				return SUCCESS;
			break;	
		}

		FILE *log = fopen("log.txt", "a");
		fprintf(log, "%c\n", c);		
		fclose(log);

	if (token.Type == 1)
		printf("TOKEN TYPE: ADD\n");
	if (token.Type == 2)
		printf("TOKEN TYPE: SUB\n");
	if (token.Type == 3)
		printf("TOKEN TYPE: MUL\n");
	if (token.Type == 4)
		printf("TOKEN TYPE: DIV\n");
	if (token.Type == 5)
		printf("TOKEN TYPE: L BRACKET\n");
	if (token.Type == 6)
		printf("TOKEN TYPE: R BRACKET\n");
	if (token.Type == 7)
		printf("TOKEN TYPE: L BRACE\n");
	if (token.Type == 8)
		printf("TOKEN TYPE: R BRACE\n");
	if (token.Type == 9)
		printf("TOKEN TYPE: COMMA\n");
	if (token.Type == 10)
		printf("TOKEN TYPE: EOF\n");
//	if (token.Type == 11)
//		printf("float\n");
	if (token.Type == 12)
		printf("TOKEN TYPE: EXPONENTIAL\n");
	if (token.Type == 13)
		printf("TOKEN TYPE: EQUAL\n");
	if (token.Type == 14)
		printf("TOKEN TYPE: NOT EQUAL\n");
	if (token.Type == 15)
		printf("TOKEN TYPE: LESS\n");
	if (token.Type == 16)
		printf("TOKEN TYPE: GREATER\n");
	if (token.Type == 17)
		printf("TOKEN TYPE: LESS EQUAL\n");
	if (token.Type == 18)
		printf("TOKEN TYPE: GREATER EQUAL\n");
	if (token.Type == 19)
		{printf("TOKEN TYPE: EOL\n");}
	if (token.Type == 20)
		printf("TOKEN TYPE: NUMBER\n");
	if (token.Type == 21)
		printf("TOKEN TYPE: ASSIGN\n");

	}
}
