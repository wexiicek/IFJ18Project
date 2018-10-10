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
				//See stateComment for reference
				int comCounter = 0;

				if (c == '\n')			{
					state = stateEOL;

				else if (isspace(c))
					state = stateStart;
				
				/*
				else if (c == '\'')
					state = stateComment;
				*/

				else if (isalpha(c) || c == '_'){
					//in our case, the identifier needs to start with lowercase - TODO!!
					/*
					if (!dynamic_string_add_char(str, (char) tolower(c)))
						return free_resources(INTERNAL, str);
					*/
					state = stateIdentifierOrKeyword;
				}
				else if (isdigit(c)){
					if (!dynamic_string_add_char(str, c)){
						return free_resources(INTERNAL, str);
					}
					state = stateNumber;
				}
				else if (c == '"')
					state = stateStringStart;
				
				else if (c == '<')
					state = stateLess;
				
				else if (c == '>')
					state = stateGreater;
				
				else if (c == '=')
					state = stateEqual;
				
				else if (c == '#')
					state = stateComment;

				else if (c == '+'){
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
				

				else if (c == '('){
					token->type = tokenLeftBracket;
					//return freeResources(tokenOK, str);
				}

				else if (c == ')'){
				token->type = tokenRightBracket;
					//return freeResources(tokenOK, str);
				}

				else if (c == ','){
					token->type = tokenComma;
					//return freeResources(tokenOK, str);
				}


				else if (c == EOF){
					token->type = tokenEOF;
					//return freeResources(tokenOK, str);
				}

				else
					//return freeResources(LEXICAL, str);
			
				break;
		



			case (sEqual):
				if (c == '='){
					token->type = tokenCompare;
					//return freeResources(tokenOK, str);
				}

				if (isspace(c))
					token->type = tokenAssign

				//TODO
				/*
					We have to solve the problem that comes with assigning a 
					variable that has name simial to "begin"..
					for example  something = begi...

				*/
				break;
			}

			case (stateComment):
			//single line comment

				if(c == '\n' || c == EOF){
					state = stateStart;
					ungetc(c, sourceCode);
				}

			break;

			case (stateBlockComment):

				if (c == 'begin') //TODO, this wont work :D

				else if (c == EOF)
					return freeResources(LEXICAL, str);

			break;

			//TODO
			case (stateStringStart):

				if (c == '"') {
					token->type = tokenEmptyString;

				}
				if (c == "")


				break;

			case (stateGreater):

				if (c == '='){
					token->type = tokenGreaterEqual
				}
				else{
					ungetc(c, sourceCode);
					token->type = tokenGreaterThan;
				}

			break;

			case (stateLess):

				if (c == '='){
					token->type = tokenLessEqual
				}
				else{
					ungetc(c, sourceCode);
					token->type = tokenLessThan;
				}

			break;
		}
	}
	return 0;
}
