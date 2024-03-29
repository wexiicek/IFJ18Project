#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "generator.h"
#include "err.h"
/*
 * Macros for colored output
*/
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"

int main( int argc, const char* argv[] ){
	(void) argc; (void) argv;
   
	#define DEBUG 1

    FILE *sourceCode;
    
	#ifdef DEBUG
		//if((sourceCode = fopen("example1.src", "r"))==NULL){
    	if((sourceCode = fopen("testfile", "r"))==NULL){
			fprintf(stderr,CRED "[MAIN]" CWHT " Couldnt open file. Stopping.\n");
			return -1;
		}
		else
			fprintf(stderr,CGRN "[MAIN]" CWHT " Debug file opened succesfully.\n");
    #else
    	sourceCode = stdin;
    #endif

    fprintf(stderr,CGRN "[MAIN]" CWHT " Launching main program.\n" CGRN "[MAIN]" CWHT " Setting source file.\n");

    if(!setSourceFile(sourceCode))
    	{fprintf(stderr,CRED"[MAIN]"CWHT" File couldnt be set.\n"); return INTERNAL;}
    fprintf(stderr,CGRN"[MAIN]"CWHT" File set properly.\n");

    int parser = kowalskiAnalysis();
    
	if(parser){
		fprintf(stderr,CRED"[MAIN]"CWHT" Error in parser.\n");
		codeGenClear();
		fclose(sourceCode);
		return parser;
	}
	else{
		codeGenPutToFile(stdout);
		fprintf(stderr,CGRN"[MAIN]"CWHT" Parser finished succesfully.\n");
	}
	fclose(sourceCode);
	return SUCCESS;
}
