#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdbool.h>

#include "string.h"
#include "err.h"


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
} *tBSTNodePtr;


void BSTInit (tBSTNodePtr *RootPtr);
tBSTNodePtr BSTSearch (tBSTNodePtr RootPtr, char *tmpKey);
int BSTInsert (tBSTNodePtr *RootPtr, char *tmpKey, void *Data, tNodeDataType nodeDataType);
void BSTDelete (tBSTNodePtr *RootPtr, char* tmpKey);
void BSTDispose (tBSTNodePtr *RootPtr);
void ReplaceByLeftmost (tBSTNodePtr *PtrReplaced, tBSTNodePtr *RootPtr);
/*------------------------------------------------- FUNCTIONS FOR SYMTABLE --------------------------------------------------*/
typedef enum{
    
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_STRING,    
    TYPE_UNDEFINED,
}dataTypeEnum;

typedef struct variable {
    dataTypeEnum dataType;
} tDataVariable;

typedef struct function {
    dataTypeEnum returnDataType;
    bool declared;
    bool defined;
    dynString parameters;   
    dynString paramName[10];
} tDataFunction;

typedef struct symtable {
    tBSTNodePtr root;
} tSymtable;


void symTableInit(tSymtable* Table);
tBSTNodePtr symTableSearch(tSymtable *Table, char *Key);
void symTableDelete(tSymtable *Table, dynString Key);
void symTableDispose(tSymtable *Table);
int symTableInsertFunction(tSymtable *Table, char *Key);
int symTableInsertVariable(tSymtable* Table, char *Key);
void Print_tree2(tBSTNodePtr TempTree, char* sufix, char fromdir);
void Print_tree(tBSTNodePtr TempTree);


#endif