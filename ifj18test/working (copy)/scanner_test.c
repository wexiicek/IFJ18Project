#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "scanner_test.h"
#include "program_data.h"
#include "string.h"



int getTokens (FILE* sourceCode) {
	if (!sourceCode)
		return INTERNAL;

	

	int state = stateStart; token.Type = tokenEmpty;
	char c;
	//Go through every character of a file
	while (1){
		token.Type = tokenEmpty;
		c = getc(sourceCode);
		switch (state){
			case (stateStart):

			/*	Cases, where we CAN determine the type definitely	*/

				if (c == '\n')
					{token.Type = tokenEndOfLine;}

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
					state = stateStringStart;
				
				else if (c == '<')
					state = stateLess;
				
				else if (c == '>')
					state = stateGreater;
				
				else if (c == '=')
					state = stateEqual;

				else if (isdigit(c)){
					if (c == '0')
						{state = stateZero; printf("debug\n");}
					else if ((c >= '1') && (c <= '9'))
						{printf("entering number1\n");
						ungetc(c, sourceCode);
						printf("entering number2\n");
						state = stateNumber;
						printf("entering number3\n");
						token.Type = tokenInteger;
						printf("entering number4\n");}
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
					{ungetc(c, sourceCode); state = stateIdentifierOrKeyword;}
				
				else 
					{token.Type = tokenEmpty; continue;}
			break;

			case (stateZero):
				return 42;
			break;

			case (stateNumber):
				if (c == '1')
					{printf("mame jednotku\n"); state = stateStart; token.Type = tokenInteger;}
				else
					state = stateStart;
			break;

			case (stateIdentifierOrKeyword):
				if ( (c == '_') || isalnum(c))
					{stringAddChar(&kwstring, c);}
				else if ( (c == '?') || (c == '!') )
					{stringAddChar(&kwstring, c); state = stateStringEnd;}
				else if (  (c == ',')  || (c == '=') || isspace(c) || (c == EOF))
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
					{state = stateStart;}
				if (c == EOF)
					{token.Type = tokenEndOfFile; state = stateEnd;}
				else
					continue;
			break;

			case (stateStringEnd):

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
		printf("add\n");
	if (token.Type == 2)
		printf("sub\n");
	if (token.Type == 3)
		printf("mul\n");
	if (token.Type == 4)
		printf("div\n");
	if (token.Type == 5)
		printf("lBr\n");
	if (token.Type == 6)
		printf("rBr\n");
	if (token.Type == 7)
		printf("llBr\n");
	if (token.Type == 8)
		printf("rrBr\n");
	if (token.Type == 9)
		printf("comma\n");
	if (token.Type == 10)
		printf("EOF\n");
	if (token.Type == 11)
		printf("float\n");
	if (token.Type == 12)
		printf("expon\n");
	if (token.Type == 13)
		printf("equal\n");
	if (token.Type == 14)
		printf("nequal\n");
	if (token.Type == 15)
		printf("less\n");
	if (token.Type == 16)
		printf("greater\n");
	if (token.Type == 17)
		printf("lEqual\n");
	if (token.Type == 18)
		printf("gEqual\n");
	if (token.Type == 19)
		{printf("EOL\n");}
	if (token.Type == 20)
		printf("num\n");
	if (token.Type == 21)
		printf("assign\n");
	if (token.Type == 22)
		printf("%s\n", kwstring.value);

	}
}
