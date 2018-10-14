#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "scanner_test.h"
#include "program_data.h"

int getTokens (FILE* sourceCode) {

	if (!sourceCode)
		return INTERNAL;

	//if (!dynamicString)
		//return INTERNAL;

	int state = stateStart; token.Type = tokenEmpty;
	char c;
	//Go through every character of a file
	while (1){
		c = getc(sourceCode);
		switch (state){
			case (stateStart):

			/*	Cases, where we CAN determine the type definitely	*/
				if (c == '+')
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

				else if (c == '\n')
					{token.Type = tokenEndOfLine;}
				
				else if (isspace(c))
					{token.Type = tokenWhitespace;}

			/*	Cases, where we CAN'T determine the type definitely	*/

				else if (c == '"')
					state = stateStringStart;
				
				else if (c == '<')
					state = stateLess;
				
				else if (c == '>')
					state = stateGreater;
				
				else if (c == '=')
					state = stateEqual;
				
				else if (c == '#')
					{state = stateComment; token.Type = tokenEmpty;}
				//Question: do we need EOL token on the end of single line
				//			comment?

				else if (c == '!')
					state = stateExclamation;
				
				else if (c == EOF)
					{return SUCCESS;}
				
				else {
					continue;}
			break;

			
			case (stateComment):
				if (c == '\n')
					{state = stateStart;}
				else
					continue;
			break;
			/* TODO

			case (stateStringStart):
				if (isalpha(c))
					{token.Value = c; strcpy(token.Type, "");}
				else
					 {printf("\n"); ungetc(c, sourceCode); state = stateStart; continue;}
				printf("%c", token.Value);
				continue;
			break;
			*/
			
		}

		printf("%d\n", token.Type);
	}
}
