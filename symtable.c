#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"
#include "err.h"
#include "scanner.h"

#define dataInit() malloc(sizeof(struct tData))

#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"

void BSTInit (tBSTNodePtr *RootPtr){
	*RootPtr = NULL;
}


tData *BSTinsertSymbol(tBSTNodePtr* RootPtr, char *k){

	if (*RootPtr == NULL){
		*RootPtr = malloc(sizeof(struct tBSTNode));
		if(RootPtr == NULL)
			return NULL;
		(*RootPtr) -> LPtr = NULL;
		(*RootPtr) -> RPtr = NULL;
		(*RootPtr) -> Key = (char *)malloc((strlen(k) + 1) * sizeof(char));
		(*RootPtr) -> Data . parameters = (dynString*) malloc(sizeof(dynString));
		stringInit((*RootPtr) -> Data . parameters);
		//fprintf(stderr, "%s\n", k);
		strcpy((*RootPtr) -> Key, k);
		fprintf(stderr, CRED"    Allocating "CWHT"%s\n", (*RootPtr) -> Key);
		(*RootPtr) -> Data . identifier = (*RootPtr) -> Key;
		(*RootPtr) -> Data . defined = false;
		(*RootPtr) -> Data . global = false;
		(*RootPtr) -> Data . dataType = TYPE_UNDEFINED;
		(*RootPtr) -> Data . argCounter = 0;
		(*RootPtr) -> Data . callArgCounter = 0;
	}

	if (strcmp(k, (*RootPtr) -> Key) > 0)
		BSTinsertSymbol (&(*RootPtr)->RPtr, k);
	else if (strcmp(k, (*RootPtr) -> Key) < 0)
		BSTinsertSymbol(&(*RootPtr)->LPtr, k);
	else 
		return NULL;

	return &((*RootPtr) -> Data);
	
}

tData *BSTsearchSymbol (tBSTNodePtr RootPtr, char* Key) {
	/*

	if(RootPtr == NULL)
		return NULL;
	
	int compare = strcmp(Key, RootPtr -> Key);
	fprintf(stderr, "Hodnota strcmp: %d\n", compare);
	if (compare == 0) {
		fprintf(stderr, "%s\n", "HELLO");
		return &RootPtr -> Data;
		} //TODO
	else if (compare > 0)
		BSTsearchSymbol(RootPtr->RPtr, Key);
	else if (compare < 0)
		BSTsearchSymbol(RootPtr->LPtr, Key);
	fprintf(stderr, "%s\n", "SIEG");
	return NULL;	
	*/
	if (RootPtr)										
	{
		if (!strcmp(RootPtr->Key, Key))								//pokud je nalezen
		{
			return &RootPtr -> Data;										//vracim TRUE
		}
		else if (strcmp(RootPtr->Key, Key) < 0)							//pokud jsem za uzlem
		{
			return(BSTsearchSymbol(RootPtr -> RPtr, Key));		//rekurzivne volam funkci posunutou zpatky
		}
		else if (strcmp(RootPtr->Key, Key) > 0)							//pokud jsem pred uzlem
		{
			return(BSTsearchSymbol(RootPtr -> LPtr, Key));		//rekurzivne volam funkci posunutou dopredu
		}
		else {													//pokud uzel neni nalezen, vracim false
			return NULL;
		}
	}
	else 
	{
		return NULL;
	}

}

void BSTdispose (tBSTNodePtr(*RootPtr)) {
	
	if (!(*RootPtr == NULL)){
		if (!((*RootPtr)->RPtr == NULL))
			BSTdispose(&(*RootPtr) -> RPtr);
			//Keep iterating through the right branch until there are no more items
		if (!((*RootPtr)->LPtr == NULL))
			BSTdispose(&(*RootPtr)->LPtr);
			//Keep iterating through the left branch until there are no more items
		if ( !((*RootPtr) -> RPtr) && (!((*RootPtr) -> LPtr))){
			free((*RootPtr) -> Key);
			stringDispose((*RootPtr) -> Data . parameters);
			free((*RootPtr) -> Data . parameters);
			free(*RootPtr);
			*RootPtr = NULL;
			fprintf(stderr, CGRN"    Disposing..\n"CWHT );
			//When there are no more items, we free the memory and set the pointer to NULL
		}
	}

	else
		return;
}


void Print_tree2(tBSTNodePtr TempTree, char* sufix, char fromdir)
/* vykresli sktrukturu binarniho stromu */

{
     if (TempTree != NULL)
     {
	char* suf2 = (char*) malloc(strlen(sufix) + 4);
	strcpy(suf2, sufix);
        if (fromdir == 'L')
	{
	   suf2 = strcat(suf2, "  |");
	  fprintf(stderr, "%s\n", suf2);
	}
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->RPtr, suf2, 'R');
       fprintf(stderr, "%s  +-[%s,%d]\n", sufix, TempTree->Key, TempTree->Data.dataType);
	strcpy(suf2, sufix);
        if (fromdir == 'R')
	   suf2 = strcat(suf2, "  |");	
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->LPtr, suf2, 'L');
	if (fromdir == 'R')fprintf(stderr, "%s\n", suf2);
	free(suf2);
    }
}

int Print_tree(tBSTNodePtr TempTree)
{
 fprintf(stderr, "Struktura binarniho stromu:\n");
 fprintf(stderr, "\n");
  if (TempTree != NULL)
     Print_tree2(TempTree, "", 'X');
  else
    fprintf(stderr, "strom je prazdny\n");
 fprintf(stderr, "\n");
 fprintf(stderr, "=================================================\n");
  return 0;
} 
