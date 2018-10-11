#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "scanner_test.h"


void getTokens (FILE* sourceCode) {
	int state = stateStart;
	char c;
	//Go through every character of a file
	while (1){
		c = getc(sourceCode);
		switch (state){
			case (stateStart):
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
				else if (isspace(c))
					{token.Type = tokenWhitespace;}
				else if (isalpha(c))
					{state = stateString; ungetc(c, sourceCode);}
				else if (c == EOF)
					{return;}
				else {
					continue;}
				break;

			/* TODO

			case (stateString):
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
