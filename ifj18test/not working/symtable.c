#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "symtable.h"
#include "scanner_test.h"


/**********************************************       BST      *******************************************/


void BSTInit (tNodePtr *RootPtr) {
    *RootPtr = NULL;
}

/*
 *  If there is an item with KEY in our table, then the function will return a pointer to it's data.
 *  Otherwise function  will return NULL
 */
tNodePtr* BSTSearch (tNodePtr *RootPtr, char *tmpKey)	{

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
        else{
            return INTERNAL;
        }      
    }
    else {
        return NULL;
    }
}

void BTSInsertNewNode(tNodePtr *RootPtr, char* Key, void* Data, tNodeDataType nodedataType){
    //Memory allocation for new node
    struct tBTSNode *newItem;
    newItem = (struct tBTSNode*)malloc(sizeof(struct tBTSNode));
        if(newItem == NULL){
            return INTERNAL;
        }
    //Data initialization for new item
    newItem->Key = Key;
    newItem->Data = Data;
    newItem->dataType = dataType;
    newItem->LPtr = NULL; 
    newItem->RPtr = NULL;

    *RootPtr = newItem;
}

/*
 * Function inserts element into symtable. If item exists, then we will update his value.
*/
void BSTInsert (tNodePtr *RootPtr, char *tmpKey, void *Data, tNodeDataType nodeDataType)	{

    if(*RootPtr == NULL){ // aktualizace dat pri nalezeni stejneho klice
        BTSInsertNewNode(*RootPtr,tmpKey,Data,nodeDataType);
        }

    else{ 
        if(strcmp(tmpKey, *RootPtr->Key) == 0){
            /*Output = 0 would mean that there is already item with this key.
              So we will just update the value of an item in the 'else' section. */
            *RootPtr->Data = Data;
        }
        else{
            if(strcmp(tmpKey, *RootPtr->Key) < 0){
                /*If our key is less than *RootPtr->Key , then we will continue searching in 
                left part of the tree. We continue in doing this using RECURSE until we find an item 
                and update it, or we will come to the end of tree and create new NODE with item. */
                BSTInsert(*RootPtr->LPtr, tmpKey, Data, NodeDataType);
            }else if(strcmp(tmpKey, (*RootPtr->Key)) > 0){
                /*If our key is more than *RootPtr->Key , then we will continue searching in 
                right part of the tree. */
                BSTInsert(*RootPtr->RPtr, tmpKey, Data, NodeDataType);
                }
        }
    }
}


void ReplaceByLeftmost (tNodePtr *PtrReplaced, tNodePtr *RootPtr){

    if(*RootPtr->LPtr == NULL){
        // prekopirovani hodnot uzlu
        PtrReplaced->Data = *RootPtr->Data;
        PtrReplaced->Key = *RootPtr->Key;
        // uvolneni uzlu
        tNodePtr *itemToDelete = *RootPtr;
        *RootPtr = *RootPtr->RPtr;
        free(itemToDelete);
    }
    else{
        ReplaceByLeftmost(PtrReplaced, &(*RootPtr->LPtr));
    }

}

void BSTDelete (tNodePtr *RootPtr, char* K){

    if (RootPtr  && RootPtr ){
        
        if(strcmp(K, RootPtr->Key) == 0){ // pokud byl nalezen uzel s danym klicem
            if((RootPtr->LPtr != NULL) && (RootPtr->RPtr != NULL)){
                ReplaceByLeftmost(RootPtr, &(RootPtr->RPtr));       // pokud ma ruseny uzel oba podstromy
            }
            else if((RootPtr->LPtr == NULL) && (RootPtr->RPtr == NULL)){ // pokud se jedna o listovy uzel
                free(RootPtr->Data); // uvolneni pameti dat
                free(RootPtr);
                RootPtr = NULL;
            }
            else if((RootPtr->RPtr != NULL) && (RootPtr->LPtr == NULL)){ // pokud ma uzel jen pravy podstrom
                free(RootPtr->Data); // uvolneni pameti dat
                free(RootPtr);
                RootPtr = RootPtr->RPtr;
            }
            else if((RootPtr->LPtr != NULL) && (RootPtr->RPtr == NULL)){ // pokud ma uzel jen levy podstrom
                free(RootPtr->Data); // uvolneni pameti dat
                free(RootPtr);
                RootPtr = RootPtr->LPtr;
            }
            else{
                return INTERNAL;
            }
        }
        else if(strcmp(K, RootPtr->Key) < 0){
            BSTDelete( &(RootPtr->LPtr), K);
        }
        else if(strcmp(K, RootPtr->Key) > 0){
            BSTDelete( &(RootPtr->RPtr), K);
        }
    }
}

