/*BASIC INCLUDES*/
	#include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
/*OPTIONAL INCLUDES*/
	#include <string.h>
	#include <math.h>
/*FILE INCLUDES*/
	#include "errors.h"
	#include "scanner.h"
/*END OF INCLUDES*/


int main( int argc, const char* argv[] ){
	//printf("The program has: %d arguments, %s\n", argc, argv[0]);
	(void) argc; (void) argv;
   
	#define DEBUG 1

    FILE *sourceCode;
    
	#ifdef DEBUG
		if((sourceCode = fopen("testfile.ifj18", "r"))==NULL){
			printf("kokotina pojebanaaaaaaaaaa");
			return -1;
		}
    #else
    	source_file = stdin;
    #endif

	setSourceCodeFile(sourceCode);

	






















	return 0;
}