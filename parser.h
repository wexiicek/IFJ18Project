#ifndef PARSER_HEADER
#define PARSER_HEADER

#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"

typedef struct {
	Token token;
	tSymtable localTable;
	tSymtable globalTable;
} parseData;

int kowalskiAnalysis();
#endif