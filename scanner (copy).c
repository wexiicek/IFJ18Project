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
					token->type = tokenEndOfFile;
					//return freeResources(tokenOK, str);
				}

				else if (c == '\n'){
					state = stateEndOfLine;
					token->type = tokenEndOfLine; //TODO
				}

				else if (isspace(c))
					state = stateStart;
				
				/*
				else if (c == '\'')
					state = stateComment;
				*/

				else if ( (isalpha(c) == ((char) tolower(c))) || (c == '_') ){
					
					if (!dynamicStringAddChar(str, c))
						return freeResources(INTERNAL, str);
				
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

				else if (c == '!')
					state = stateExclamation;
			

				

				else
					//return freeResources(LEXICAL, str);
			
				break;
		


			/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&ccc*/



			/* FINISHED STATES*/
			case (stateLineComment):
			//single line comment
				if (c == '\n')
					state = stateStart;

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


			/* /FINISHED STATES*/
			
			case (stateExclamation):

				if (c == '=') {
					token->type = tokenNotEqual
					//return freeResources(tokenOK, str);
				}
				else {
					return LEXICAL; //syntactical??? TODO
				}

			break;


			
			case (stateEqual):
				//we need to add comment support TODO
				if (c == '='){
					token->type = tokenCompare;
					//return freeResources(tokenOK, str);
				}
				else if (c != '=') {
					token->type = tokenAssign;
				}




			case (stateStringStart):

				if (c == '"') {
					token->type = tokenEmptyString;

				}
				else if (c != '\n') {
					if (!dynamic_string_add_char(str, c))
						freeResources(INTERNAL, str);
					state = stateStringContinue;
				}


			break;

			case (stateStringContinue):

				if (c == '"') {
					token->type = tokenUnemptyString;

				}
				else if (c != '\n') {
					if (!dynamic_string_add_char(str, c))
						freeResources(INTERNAL, str);
					state = stateStringContinue;
				}


			break;

			/* UNFINISHED STATES*/

				

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
			
			/* / UNFINISHED STATES*/
		}
	}
	return 0;
}
