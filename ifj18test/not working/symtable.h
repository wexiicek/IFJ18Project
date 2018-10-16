#include <stdbool.h>

#include "string.h"

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

/* ----------------------------------------BINARY SEARCH(ING??????????) TREE-------------------------------------------------*/


typedef enum {
    ndtVariable,     // nodeDataTypeVariable 
    ndtFunction,     // nodeDataTypeFunction 
} tNodeDataType;

/*
 * Node of our tree
 */
typedef struct tNode {
    char* Key;			                             
    tNodeDataType nodeDataType;  //Data type of the node, see out enum
    void* Data;                  //Place where to save our data
    struct tNode *LPtr;          //Pointer to the left part of the tree
    struct tNode *RPtr;          //Pointer to the right part of the tree
} *tNodePtr;

/*
 * Function prototypes
 */
void BSTInit   (tNodePtr *);
void BTSInsertNewNode(tNodePtr* , char* , void* , tNodeDataType);
void BSTInsert (tNodePtr *, char*, void*, tNodeDataType);
tNodePtr BSTSearch (tNodePtr, char*);
void BSTDelete (tNodePtr *, char*);
void BSTDispose(tNodePtr *);

#endif