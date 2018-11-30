#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdbool.h>
#include "string.h"
#include "err.h"

typedef enum{
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_UNDEFINED,
} dataTypeEnum;

typedef struct tData{
    dataTypeEnum dataType;
    bool defined;
    bool global;
    dynString *parameters;   
    char *identifier;
} tData; //TODO hvezdicka?
                                                                                                            
typedef struct tBSTNode {
	char *Key;
	tData Data;
	struct tBSTNode * LPtr;
	struct tBSTNode * RPtr;
} *tBSTNodePtr;	

void BSTInit (tBSTNodePtr *);
tData *BSTinsertSymbol (tBSTNodePtr *, char *k); //, tData
tData *BSTsearchSymbol (tBSTNodePtr, char*);
void BSTdispose (tBSTNodePtr*);
void Print_tree2(tBSTNodePtr TempTree, char*, char fromdir);
int Print_tree(tBSTNodePtr TempTree);
void freeTree(tBSTNodePtr *);

#endif