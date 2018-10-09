/*BASIC INCLUDES*/
	#include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
/*OPTIONAL INCLUDES*/
	#include <string.h>
	#include <math.h>
/*FILE INCLUDES*/
	#include "errors.h"
/*END OF INCLUDES*/

/*
	Base main file for IFJ18 project.
*/
char tokens[100];

void splitString(char* input){
	char* kokotina = input;
	int dlzka = strlen(kokotina);
	
	for (int i = 0; i < dlzka; i++)
		tokens[i] = input[i];
}


int main( int argc, const char* argv[] ){
	//printf("The program has: %d arguments, %s\n", argc, argv[0]);
	(void) argc;
	(void) argv;

	char* string = "print \"hello world\"";

	splitString(string);

	//int arrSize = sizeof(tokens)/sizeof(char);

	int counter = 0;
	for (int i = 0; i < 100; i++)
		if(isalpha(tokens[i])){
			printf("%c\n",tokens[i]);
			counter++;
		}

	char* compare = "print";
char helper[] = "";

int helpCount = 0;
	for (int i = 0; i < counter; i++){
		
		helper[helpCount] = tokens[i];
		helpCount++;
		if(strcmp(helper, compare) == 0){
			printf("hooooray\n");
			strcpy(helper, "");
			helpCount = 0;
		}
	}

	






















	return 0;
}