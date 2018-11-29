#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdbool.h>

#include "string.h"
#include "err.h"
#define TRUE 1
#define FALSE 0

extern int solved;                        /* indikace, zda byla funkce řešena */

/* uzel stromu */

typedef enum{
    TYPE_INTEGER,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_UNDEFINED,
}dataTypeEnum;

typedef struct tData{
    dataTypeEnum dataType;
    bool defined;
    bool global;
    dynString *parameters;   
    char *identifier;
} tData;
                                                                                                            
typedef struct tBSTNode {
	char *Key;			                                                      /* klíč */
	tData Data;                                            /* užitečný obsah uzlu */
	struct tBSTNode * LPtr;                                    /* levý podstrom */
	struct tBSTNode * RPtr;                                   /* pravý podstrom */
} *tBSTNodePtr;	

/* prototypy funkcí */

void BSTInit   (tBSTNodePtr *);
tData* BSTSearch  (tBSTNodePtr, char*);
void BSTInsert (tBSTNodePtr *, char*, tData);
void BSTDelete (tBSTNodePtr *, char);
void BSTDispose(tBSTNodePtr *);

/* konec c401.h */
#endif