#include <stdbool.h>

#include "string.h"

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

#endif