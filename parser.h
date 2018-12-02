#ifndef PARSER_HEADER
#define PARSER_HEADER

#include <stdbool.h>

#include "scanner.h"
#include "symtable.h"

int setDestFile(FILE *destFile);

#define addToOutput(_funCall, ...)\
			_funCall(__VA_ARGS__)

typedef struct {
	Token token;
	Token prevToken;
	tBSTNodePtr localTable;
	tBSTNodePtr globalTable;

	tData* currentID;
	tData* lID;
	tData* rID;

	unsigned parameterIndex;
	int labelIndex;
	int labelDeep;

	bool inFunction;
	bool inDecleration;
	bool inWhileOrIf;
	bool nonDeclaredFunction;
} parseData;

int kowalskiAnalysis();
#endif