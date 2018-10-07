#include <stdio.h>
#include "errors.h"

const char *errorTable[] = {
	[LEXICAL] = "There was a lexical error in the program.\n",
	[SYNTACTICAL] = "There was a syntactical error in the program.\n",
	[SEMANTICAL] = "There was a semantical error in the program.\n",
	[COMPATIBILITY] = "There was a compatibility error in the program.\n",
	[OTHER] = "Other problem in the program.\n",
	[ZERO_DIV] = "Division by zero.\n",
	[INTERNAL] = "There was an internal compiler error.\n"
};

int printError(int ret_msg_id){
	printf(stderr, "%s", errorTable[errorID]);
	return errorID;
}