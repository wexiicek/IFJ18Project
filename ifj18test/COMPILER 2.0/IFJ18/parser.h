#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <stdbool.h>

#include "scanner.h"

typedef struct {
	Token token;
} parseData;

int kowalskiAnalysis();
#endif