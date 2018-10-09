#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "scanner.h"
#include "error.h"
#include "strlib.h"
#include "clear.h"

#define DEBUG 1

FILE *sourceCode;

int main (){

    FILE *sourceCode;
    #ifdef DEBUG
	    if (!(sourceCode = fopen("test.bas", "r"))){
		    source_Code = stdin;
        }
    #else
    	source_file = stdin;
    #endif

}