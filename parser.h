#ifndef PARSER_HEADER
#define PARSER_HEADER

#include <stdbool.h>

#include "scanner.h"

typedef struct {
	Token token;
} parseData;

int kowalskiAnalysis();
#endif