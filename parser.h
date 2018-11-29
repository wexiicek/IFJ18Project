#ifndef PARSER_HEADER
#define PARSER_HEADER

#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"

int setDestFile(FILE *destFile);

typedef struct {
	Token token;
	tBSTNodePtr localTable;
	tBSTNodePtr globalTable;
} parseData;

int kowalskiAnalysis();
#endif