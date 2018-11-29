
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů 
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "symtable.h"
#include "scanner.h"
#include "err.h"


void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**	
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.	
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/
	
	
	*RootPtr = NULL; //strom inicializuji na NULL
	
}	

tData* BSTSearch (tBSTNodePtr RootPtr, char* K)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/
							   
	
	if (RootPtr)										
	{
		if (!strcmp(RootPtr->Key, K))								//pokud je nalezen
		{
			return &RootPtr -> Data;										//vracim TRUE
		}
		else if (strcmp(RootPtr->Key, K) < 0)							//pokud jsem za uzlem
		{
			return(BSTSearch(RootPtr -> RPtr, K));		//rekurzivne volam funkci posunutou zpatky
		}
		else if (strcmp(RootPtr->Key, K) >0)							//pokud jsem pred uzlem
		{
			return(BSTSearch(RootPtr -> LPtr, K));		//rekurzivne volam funkci posunutou dopredu
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


void BSTInsert (tBSTNodePtr* RootPtr, char* K, tData Content)	{	
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
		
	
	if (!(*RootPtr))
	{
		(*RootPtr) = malloc(sizeof(struct tBSTNode));		//alokuji misto pro novy prvek
		(*RootPtr) -> Key = K;								//naplneni noveho prvku
		(*RootPtr) -> Data.defined = FALSE;
		(*RootPtr) -> Data.global = FALSE;
		(*RootPtr) -> LPtr = NULL;
		(*RootPtr) -> RPtr = NULL;

		//tBSTNodePtr *new = 
	}
	else
	{

		if (!strcmp((*RootPtr)->Key, K))							//pokud prvek s klicem existuje, prepisu hodnotu
		{
			(*RootPtr) -> Data = Content;
		}
		else if (strcmp((*RootPtr)->Key, K) > 0)						//presun zpatky
		{
			BSTInsert(&(*RootPtr) -> LPtr, K, Content);
		}
		else if (strcmp((*RootPtr)->Key, K) < 0)						//presun dopredu
		{
			BSTInsert(&(*RootPtr) -> RPtr, K, Content);
		}
	}
	
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně. 
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete(). 
**/
	
	
	if (*RootPtr)
	{
		if ((*RootPtr) -> RPtr)												//presunu si do nejpravejsiho uzlu
		{
			ReplaceByRightmost(PtrReplaced, &(*RootPtr) -> RPtr);
		}
		else																//nahrazeni hodnoty
		{
			PtrReplaced -> Key = (*RootPtr) -> Key;
			PtrReplaced -> Data = (*RootPtr) -> Data;
			tBSTNodePtr temporary;											//pomocna promenna
            temporary = (*RootPtr);
			(*RootPtr) = (*RootPtr)->LPtr; 
			free(temporary);												//uvolneni pameti
		}
	} 
	
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic. 
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/
	
	
	if (*RootPtr) {
		if (*(*RootPtr) -> Key == K) {								//nalezeni odpovidajiciho uzlu
			if (((*RootPtr)->LPtr) && ((*RootPtr)->RPtr)) 			//pokud ma pravy i levy podstrom
			{
				ReplaceByRightmost((*RootPtr), &(*RootPtr)->LPtr);
		 	}
			else if ((*RootPtr)->RPtr) 								//pokud ma pravy podstrom
			{
				tBSTNodePtr temporary;
                temporary = *RootPtr;
				*RootPtr = (*RootPtr)->RPtr; 
				free(temporary);
			} 
			else if ((*RootPtr)->LPtr)								//pokud ma levy podstrom
			{
				tBSTNodePtr temporary;
                temporary = *RootPtr;
				*RootPtr = (*RootPtr)->LPtr; 
				free(temporary);
			} 
			else 													//pokud nema podstrom
			{
				free(*RootPtr);
				*RootPtr = NULL;
			}
		}
		else 														//klic nebyl nalezen
		{
			if (*(*RootPtr)->Key > K) {								//pokracuje doleva
				BSTDelete(&(*RootPtr)->LPtr, K);	
			} 
			else if (*(*RootPtr)->Key < K)							//pokracuje doprava
			{
				BSTDelete(&(*RootPtr)->RPtr, K);
			}
		}
	} 
	else 
	{
		return;
	}

} 

void BSTDispose (tBSTNodePtr *RootPtr) {	
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/
	
	if (*RootPtr)
	{
		if ((*RootPtr) -> LPtr)									//leva vetev
		{
			BSTDispose (&(*RootPtr) -> LPtr);
		}
			
		if ((*RootPtr) -> RPtr)									//prava vetev
		{
			BSTDispose (&(*RootPtr) -> RPtr);
		}
			
		if (!((*RootPtr) -> RPtr) && !((*RootPtr) -> LPtr))		//zruseni stromu
		{
			free(*RootPtr);										//uvolneni pameti
			*RootPtr = NULL;
			return;
		}
	}

}

/* konec c401.c */

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
	   printf("%s\n", suf2);
	}
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->RPtr, suf2, 'R');
        printf("%s  +-[%s,%d]\n", sufix, TempTree->Key, TempTree->Data.dataType);
	strcpy(suf2, sufix);
        if (fromdir == 'R')
	   suf2 = strcat(suf2, "  |");	
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->LPtr, suf2, 'L');
	if (fromdir == 'R') printf("%s\n", suf2);
	free(suf2);
    }
}

void Print_tree(tBSTNodePtr TempTree)
{
  printf("Struktura binarniho stromu:\n");
  printf("\n");
  if (TempTree != NULL)
     Print_tree2(TempTree, "", 'X');
  else
     printf("strom je prazdny\n");
  printf("\n");
  printf("=================================================\n");
} 