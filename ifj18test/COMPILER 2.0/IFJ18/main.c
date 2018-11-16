#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"

int main( int argc, const char* argv[] ){
	(void) argc; (void) argv;
   
	#define DEBUG 1

    FILE *sourceCode;
    
	#ifdef DEBUG
		if((sourceCode = fopen("testfile", "r"))==NULL){
			printf(CRED "[MAIN]" CWHT " Couldnt open file. Stopping.\n");
			return -1;
		}
		else
			printf(CGRN "[MAIN]" CWHT " Debug file opened succesfully.\n");
    #else
    	sourceCode = stdin;
    #endif

    printf(CGRN "[MAIN]" CWHT " Launching main program.\n" CGRN "[MAIN]" CWHT " Setting source file.\n");

    if(!setSourceFile(sourceCode))
    	{printf(CRED"[MAIN]"CWHT" File couldnt be set.\n"); return 1;}
    printf(CGRN"[MAIN]"CWHT" File set properly.\n");

	if(kowalskiAnalysis())
		printf(CRED"[MAIN]"CWHT" Error in parser.\n");
	else
		printf(CGRN"[MAIN]"CWHT" Parser finished succesfully.\n");

	return 0;
}
