#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "scanner_test.h"


//Initialize the structure
Tokens token;

void getTokens (FILE* sourceCode) {

	char c;
	//Go through every character of a file
	while (1){
		c = getc(sourceCode);
		if (c == '+')
			{token.Value = c; strcpy(token.Type,"Addition");}
		else if (c == '-')
			{token.Value = c; strcpy(token.Type,"Subtraction");}
		else if (c == '*')
			{token.Value = c; strcpy(token.Type,"Multiplication");}
		else if (c == '/') 
			{token.Value = c; strcpy(token.Type,"Divison");}
		else if (c == '(')
			{token.Value = c; strcpy(token.Type, "Left Bracket");}
		else if (c == ')')
			{token.Value = c; strcpy(token.Type, "Right Bracket");}	
		else if (isalpha(c))
			{token.Value = c; strcpy(token.Type, "string");}
		else if (c == EOF)
			{return;}

		else {
			continue;

		}
		if(strcmp(token.Type, "string") == 0)
			{printf("%c", token.Value); continue;}

		printf("%c %s\n", token.Value, token.Type);
	}
}
