#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define DEBUG 1

FILE *sourceCode;

void setSourceCodeFile (FILE *f){
    printf("debug\n");

    sourceCode = f;

    printf("debug1\n");
}

void set_dynamic_string(dynamiString *string){
	dynamicString = string;
}

int getNextToken (Token *token){
	if (sourceCode == NULL)
		return INTERNAL;
	//If there is no code file, we return INTERNAL error

	if (dynamicString == NULL)
		return INTERNAL;
	//If we failed to allocate memory for dynamic string, we return INTERNAL error

	token -> attribute.string = dynamicString;
	//dunno TODO

	dynamicString string;
	dynamicString *str = &string;

	if(!dynamicStringInit(str))
		return INTERNAL;
		//If we failed, TODO


	int state = sStart;
	token -> type = tokenEmpty;

	char c; *endptr; strnum[4] = {0};

	//reading chars from sourceCode
	while(1){
		c = (char) getc(sourceCode);
		switch (state){
			case (sStart):

				//These have possible midstates
				if (c == '\n')
					state = sEOL;
				
				else if (isspace(c))
					state = sSpace; //TODO
				
				else if (c == '\\') {
					//TODO
				}

				else if (c == '=')
					state = sEqual;
				
				else if (c == '>')
					state = sGreater;
				
				else if (c == '<')
					state = sLess;
				
				else if (c == '!')
					state = sExclamation;

				else if (c == '"')
					state = sStringStart;





				//These are definitive 
				else if (c == '#')
					token -> type = tokenComment;

				else if (c == '(')
					token -> type = tokenLeftBracket;

				else if (c == ')')
					token -> type = tokenRightBracket;

				else if (c == '+')
					token -> type = tokenAdd;
				else if (c == '-')
					token -> type = tokenSub; // could be negative number TODO
				else if (c == '*')
					token -> type = tokenMul;
				else if (c == '/')
					token -> type = tokenDiv;






				else if (isdigit(c)){
					//TODO
				}
			



			case (sEqual):
				sState = ;

			case (sStringStart){
				if (c == '"')
					token -> type = tokenEmptyString;
			}
		}
	}
	return 0;
}
