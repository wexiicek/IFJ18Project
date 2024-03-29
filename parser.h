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
	tData* tempCurrentID;
	tData* lID;
	tData* rID;
	tData* tmp;

	unsigned parameterIndex;
	int labelIndex;
	int labelDeep;

	bool inPrint;
	bool inFunction;
	bool inWhileOrIf;
} parseData;

int kowalskiAnalysis();
#endif