#include <stdbool.h>

#include "string.h"
#include "errors.h"

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

/* ----------------------------------------BINARY SEARCH(ING??????????) TREE-------------------------------------------------*/


typedef enum {
    ndtVariable,     /* nodeDataTypeVariable */
    ndtFunction,     /* nodeDataTypeFunction */
} tNodeDataType;

/*
 * Tree node
 */
typedef struct tBSTNode {
    char* Key;			                             
    tNodeDataType nodeDataType;                      
    void* Data;                                      
    struct tBSTNode * LPtr;                          
    struct tBSTNode * RPtr;                          
} *tNodePtr;


void BSTInit   (tBSTNodePtr *);
tBSTNodePtr BSTSearch (tBSTNodePtr, char*);
void BSTInsert (tBSTNodePtr *, char*, void*, tNodeDataType);
void BSTDelete (tBSTNodePtr *, char*);
void BSTDispose(tBSTNodePtr *);
/*------------------------------------------------- FUNCTIONS FOR SYMTABLE --------------------------------------------------*/

typedef struct variable {
    int dataType; /* sInteger, sFloat, sString */
} tDataVariable;

typedef struct function {
    int returnDataType; /* sInteger, sFloat, sString */
    bool declared;
    bool defined;
    string parameters;   
    string paramName[10];
} tDataFunction;

typedef struct symtable {
    tBSTNodePtr root;
} tSymtable;


void symTableInit(tSymtable*);
tBSTNodePtr symTableSearch(tSymtable*, string);
void symTableDelete(tSymtable*, string);
void symTableDispose(tSymtable*);
void symTableInsertFunction(tSymtable*, string);
void symTableInsertVariable(tSymtable*, string);


#endif