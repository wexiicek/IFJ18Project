#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define DEBUG 1

FILE *sourceCode;

void setSourceCodeFile (FILE *f){
    printf("Setting up file..\n");

    sourceCode = f;

    printf("Finished. \n");
}
/*
void set_dynamic_string(dynamiString *string){
	dynamicString = string;
}
*/

int getNextToken (Token *token){
	if (sourceCode == NULL)
		return INTERNAL;
	//If there is no code file, we return INTERNAL error
/*
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
*/
	

	int state = sStart;
	token -> type = tokenEmpty;

	char c; //*endptr; strnum[4] = {0};

	//reading chars from sourceCode
	while(1){
		c = (char) getc(sourceCode);
		switch (state){
			case (sStart):

				if (c == '+'){
					token->type = tokenAdd;
					//return freeResources(tokenOK, str);
				}

				else if (c == '-'){
					token->type = tokenSub;
					//return freeResources(tokenOK, str);
				}

				else if (c == '*'){
					token->type = tokenMul;
					//return freeResources(tokenOK, str);
				}


				else if (c == '/') {
					token->type = tokenDiv;
					//return freeResources(tokenOK, str);
				}
			}
		}
	return 0;
}
