#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "symtable.h"
#include "scanner.h"
#include "err.h"


/**********************************************       BST      *******************************************/


void BSTInit (tBSTNodePtr *RootPtr) {
    *RootPtr = NULL;
}

/*
 *  If there is an item with KEY in our table, then the function will return a pointer to it's data.
 *  Otherwise function  will return NULL
 */
tBSTNodePtr BSTSearch (tBSTNodePtr RootPtr, char *tmpKey)	{

    if(RootPtr != NULL) {
        if(strcmp(tmpKey, RootPtr->Key) == 0){
            return RootPtr;
        }
        else if(strcmp(tmpKey, RootPtr->Key) < 0){
            return BSTSearch(RootPtr->LPtr, tmpKey);    //We will continue in Left part of the tree
        }
        else if (strcmp(tmpKey, RootPtr->Key) > 0) {
            return BSTSearch(RootPtr->RPtr, tmpKey);    //We will continue in right part of the tree
        }
        /*
        else{
            return INTERNAL;
        }      
        */
    }
    else {
        return NULL;
    }
    return SUCCESS;
}

/*
 * Function inserts element into symtable. If item exists, then we will update his value.
*/
int BSTInsert (tBSTNodePtr *RootPtr, char *tmpKey, void *Data, tNodeDataType nodeDataType)	{

    if(*RootPtr == NULL){ // aktualizace dat pri nalezeni stejneho klice
        struct tBSTNode *newItem;
        newItem = (struct tBSTNode*)malloc(sizeof(struct tBSTNode));
        
        if(newItem == NULL){
            return INTERNAL;
        }
    
    //Data initialization for new item
    newItem->Key = tmpKey;
    newItem->Data = Data;
    newItem->nodeDataType = nodeDataType;
    newItem->LPtr = NULL; 
    newItem->RPtr = NULL;
    *RootPtr = newItem;
    }

    else{ 
        if(strcmp(tmpKey, (*RootPtr)->Key) == 0){
            /*Output = 0 would mean that there is already item with this key.
              So we will just update the value of an item in the 'else' section. */
            (*RootPtr)->Data = Data;
        }
        else{
            if(strcmp(tmpKey, (*RootPtr)->Key) < 0){
                /*If our key is less than *RootPtr->Key , then we will continue searching in 
                left part of the tree. We continue in doing this using RECURSE until we find an item 
                and update it, or we will come to the end of tree and create new NODE with item. */
                BSTInsert(&((*RootPtr)->LPtr), tmpKey, Data, nodeDataType);
            }else if(strcmp(tmpKey, ((*RootPtr)->Key)) > 0){
                /*If our key is more than *RootPtr->Key , then we will continue searching in 
                right part of the tree. */
                BSTInsert(&((*RootPtr)->RPtr), tmpKey, Data, nodeDataType);
                }
        }
    }
    return SUCCESS;
}


void ReplaceByLeftmost (tBSTNodePtr *PtrReplaced, tBSTNodePtr *RootPtr){

    if((*RootPtr)->LPtr == NULL){
        // We need to copy all values of the node
        (*PtrReplaced)->Data = (*RootPtr)->Data;
        (*PtrReplaced)->Key = (*RootPtr)->Key;
        // Here we want to free the node
        tBSTNodePtr itemToDelete = (*RootPtr);
        (*RootPtr) = (*RootPtr)->RPtr;
        free(itemToDelete);
    }
    else{
        ReplaceByLeftmost(PtrReplaced, &((*RootPtr)->LPtr));
    }

}

void BSTDelete (tBSTNodePtr *RootPtr, char* tmpKey){

    if (RootPtr && (*RootPtr)){
        if (strcmp(tmpKey,(*RootPtr)->Key) == 0){    // If we found the node with it's key
            if ((*RootPtr)->LPtr == NULL){
                if(((*RootPtr)->RPtr == NULL)){      // If it's a leaf
                    free((*RootPtr)->Data);          
                    free(*RootPtr);
                    *RootPtr = NULL;
                }
                if((*RootPtr)->RPtr != NULL){        // If the node has only the right side
                    free((*RootPtr)->Data);          
                    free(*RootPtr);
                    *RootPtr = (*RootPtr)->RPtr;
                }
            }
            if((*RootPtr)->RPtr == NULL){
                if(((*RootPtr)->LPtr == NULL)){      // If it's a leaf
                    free((*RootPtr)->Data);          
                    free(*RootPtr);
                    *RootPtr = NULL;
                }
                if((*RootPtr)->LPtr != NULL){        //If the node has only left side 
                    free((*RootPtr)->Data);          
                    free(*RootPtr);
                    *RootPtr = (*RootPtr)->LPtr;
                }
            }
            if((*RootPtr)->LPtr != NULL){            // If the node has both sides
                if((*RootPtr)->RPtr != NULL){
                    ReplaceByLeftmost(RootPtr, &((*RootPtr)->RPtr));
                }
            }
        }
        else if(strcmp(tmpKey,(*RootPtr)->Key) < 0){ //Here we continue searching in the left side
            BSTDelete(&((*RootPtr)->LPtr), tmpKey);
        }
        else if(strcmp(tmpKey,(*RootPtr)->Key) > 0){ //Here in the right side
            BSTDelete(&((*RootPtr)->RPtr), tmpKey);
        }
    }
}

void BSTDispose (tBSTNodePtr *RootPtr) {
if ((*RootPtr) != NULL){
        BSTDispose(&(*RootPtr)->LPtr); //Canceling left side of the tree
        BSTDispose(&(*RootPtr)->RPtr); //Canceling right side of the tree
        //From here till the end we are doing operations to free a single item 
        free((*RootPtr)->Key); //We have to free the key
        (*RootPtr)->Key = NULL;
        //If it's a function
        if ( (*RootPtr)->nodeDataType == ndtFunction ) {
            stringDispose(&(((tDataFunction*)(*RootPtr)->Data)->parameters));
        }

        free((*RootPtr)->Data); //Then we have to free it's data
        (*RootPtr)->Data = NULL;

        free(*RootPtr); //And finally free the node
        *RootPtr = NULL;
    }

}
/*******************************  SYMTABLE  **********************************************/

/*
 * Initialization
 */
void symTableInit(tSymtable* Table) {
    BSTInit(&(Table->root));
}

/*
 * Function will search for an item in symtable
 * Function returns a pointer on the node
 * If function does not find an item, then returns NULL
 */
tBSTNodePtr symTableSearch(tSymtable *Table, char *Key) { 
    return BSTSearch(Table->root, Key);
}

/*
 * Function deletes an item from symtable
 */
void symTableDelete(tSymtable *Table, dynString Key) {
    BSTDelete(&(Table->root), Key.value);
}

/*
 * Function deletes whole symtable
 */
void symTableDispose(tSymtable *Table) {
    BSTDispose(&(Table->root));
}

/*
 * Function inserts a data about a function into the symtable
 */
int symTableInsertFunction(tSymtable *Table, dynString Key){ 
    // Memory allocation for data
    tDataFunction *dataPtr;
    dataPtr = (tDataFunction*)malloc(sizeof(tDataFunction)); 
    if(dataPtr == NULL){
        return INTERNAL;
    }
    // Data initialization
    dynString parameters;
    stringInit(&parameters);
    dataPtr->returnDataType = -1;
    dataPtr->declared = false;
    dataPtr->defined = false;
    dataPtr->parameters = parameters;
    // Here we create a new intem in symtable
    BSTInsert(&(Table->root), Key.value, dataPtr, ndtFunction);
    return SUCCESS;
}

/*
 *  Function inserts data about a variable
 */
int symTableInsertVariable(tSymtable* Table, dynString Key) { 
    // Memory allocation
    tDataVariable * dataPtr;
    dataPtr = (tDataVariable*)malloc(sizeof(tDataVariable));
    if(dataPtr == NULL){
        return INTERNAL;
    }
    
    dataPtr->dataType = -1;
    // Here we create a new intem in symtable
    BSTInsert(&(Table->root), Key.value, dataPtr, ndtVariable);
    return SUCCESS;
}

