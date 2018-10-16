#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "scanner_test.h"
#include "program_data.h"
#include "string.h"

int getTokens (FILE* sourceCode) {

	if (!sourceCode)
		return INTERNAL;

	//if (!dynamicString)
		//return INTERNAL;

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
					{state = stateEOL;}

				else if (isspace(c))
					{state = stateStart;}

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

		
				//else if (isspace(c))
				//	{state = stateStart; token.Type = tokenEmpty;}

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
					if (c == 0)
						state = stateZero;
					else
						state = stateNumber;
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
				if ((!isspace(c)) || (c == '.'))
					{ungetc(c, sourceCode); state = stateNumber;}
				return LEXICAL;
			break;
/*
			case (stateNumber):
				 if (c == '.')
					{token.Type = tokenFloat;}
				
				else if ( (c == 'e') || (c == 'E') )
					//{stringAddChar(nejakyStringKteryEsteNemame, c); token.Type = tokenExponential;}

				else if (isspace(c))
					{state = stateStart; }

				//stringAddChar(nejakyStringKteryEsteNemame, c);



			break;

			case (stateIdentifierOrKeyword):
				if ( (c == '_') || isalnum(c))
					stringAddChar(nejakyStringKteryEsteNemame, c);
				else if ( (c == '?') || (c == '!') )
					{stringAddChar(nejakyStringKteryEsteNemame, c); state = stateIdentifierCheck;}
*/
			break;

			case (stateEOL):
				if (isspace(c))
					break;
				token.Type = tokenEndOfLine;
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
					{token.Type = tokenLessEqual;}
				else
					{ungetc(c, sourceCode); token.Type = tokenLess;}
			break;

			case (stateGreater):
				if (c == '=')
					{token.Type = tokenGreaterEqual;}
				else
					{token.Type = tokenGreater; state = stateStart;}
			break;

			case (stateExclamation):
				if (c == '=')
					{token.Type = tokenNotEqual;}
				else
					return LEXICAL;
			break;

			case (stateComment):
				if (c == '\n')
					{state = stateStart;}
				if (c == EOF)
					{token.Type = tokenEndOfFile;}
				else
					continue;
			break;		

			case (stateEnd):
				return SUCCESS;
			break;	
		}

	if (token.Type == 0)
		printf("empty\n");
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
		printf("EOL\n");
	if (token.Type == 20)
		printf("num\n");
	if (token.Type == 21)
		printf("assign\n");

	}
}
