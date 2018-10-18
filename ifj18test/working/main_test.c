#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner_test.h"
#include "program_data.h"

int main( int argc, const char* argv[] ){
	//printf("The program has: %d arguments, %s\n", argc, argv[0]);
	(void) argc; (void) argv;
   
	//#define DEBUG 1

    FILE *sourceCode;
    
	#ifdef DEBUG
		if((sourceCode = fopen("mrdka", "r"))==NULL){
			printf("kokotina pojebanaaaaaaaaaa\n");
			return -1;
		}
    #else
    	sourceCode = stdin;
    #endif

	if (getTokens(sourceCode) == 42)
		printf("vratil sa vesmir\n");

	return 0;
}
