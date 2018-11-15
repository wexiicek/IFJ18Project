#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner_test.h"

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

	if(kowalsiAnalyze())
		printf("sme pici\n");
	else
		printf("kokot funguje to\n");

	return 0;
}
