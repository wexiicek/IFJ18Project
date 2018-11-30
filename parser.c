#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "string.h"
#include "parser.h"
#include "err.h"
#include "expression.h"
#include "generator.h"
#include "symtable.h"

/*
 * Macros for colored output
*/
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"



/*
 * FORWARD DECLARATION
 * - So that they can be called before they were declared
*/
static int params(parseData *parserData);
static int params_n(parseData* parserData);
static int body(parseData* parserData);
static int terms(parseData* parserData);
static int terms_n(parseData* parserData);
static int def_value(parseData* parserData);
static int args(parseData* parserData);
static int args_n(parseData* parserData);
static int func(parseData* parserData);


FILE* output;
/*
 * Macro declaration
 * Makes our work easier and keeps the code clean and understandable
*/
#define getToken()\
	getTokens(&parserData->token)

#define checkRule(rule)\
	if((res = rule(parserData))) return res

#define checkTokenType(_type)\
	if(!(parserData -> token.Type == (_type))) return SYNTACTICAL

#define checkTokenType2(_type, _type2)\
	if (parserData -> token.Type == (_type2)) return 0;\
	else if (!(parserData -> token.Type == (_type))) return SYNTACTICAL

#define checkMultipleTokenType(_type, _type2, _type3, _type4)\
		if (!(parserData -> token.Type == (_type)) && !(parserData -> token.Type == (_type2)) &&\
		    !(parserData -> token.Type == (_type3)) && !(parserData -> token.Type == (_type4))) return SYNTACTICAL

#define checkKeyword(_keyword)\
	if(parserData -> token.Type != tokenKeyword\
	   || parserData -> token.Data.keyword != (_keyword)) return SYNTACTICAL


char* tempString;


static int mainFun(parseData* parserData){
	//fprintf(stderr,CRED "    <MAINFUN>\n"CWHT);
	//fprintf(stderr,"    %d, %d\n", parserData -> token.Type, parserData -> token.Data.keyword);
	int res;
	
	//<main> -> DEF ID ( <params> ) EOL <body> END EOL <main>
	if((parserData -> token.Type == tokenKeyword) && (parserData->token.Data.keyword == KW_DEF)){
		//Checking function definition

		getToken();
		//fprintf(stderr, "print %d\n", Print_tree(parserData->globalTable));
		//fprintf(stderr, "	we have		%s %d %d\n", parserData->token.Data.string->value, parserData->token.Type, parserData->token.Data.string->length);
		checkTokenType(tokenIdentifier);
		
		//tempString = malloc(sizeof(char)*(strlen(parserData->token.Data.string->value)+1));

		//strcpy(tempString, parserData->token.Data.string->value);
		if (BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value) == NULL)
		{

			BSTinsertSymbol(&parserData->globalTable, parserData->token.Data.string->value);

		}
		else{
			return 42;
		}

		parserData->currentID = BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value);
		//fprintf(stderr, "%s\n", parserData->currentID->identifier);
		addToOutput(codeGenFuncBegin, parserData->currentID->identifier);
		

		Print_tree(parserData->globalTable);
		getToken();
		checkTokenType(tokenLeftBracket);
		
		getToken();
		
		//Check parameter syntax
		checkRule(params);
		
		//Check body of the function
		checkTokenType(tokenRightBracket);
		
		getToken();
		checkTokenType(tokenEndOfLine);
		
		getToken();
		checkRule(body);
		
		checkKeyword(KW_END);
		addToOutput(codeGenFuncEnd, parserData->currentID->identifier);
		BSTdispose(&parserData->localTable);
		getToken();
		checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		getToken();

		return (mainFun(parserData));
	}

	else if (parserData -> token.Type == tokenEndOfLine){
		getToken();

		return mainFun(parserData);
	}

	else if (parserData -> token.Type == tokenEndOfFile){
		return SUCCESS;
	}


	else if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenEndOfLine || parserData -> token.Data.keyword == KW_IF || parserData -> token.Data.keyword == KW_WHILE || parserData -> token.Data.keyword == KW_PRINT){
		checkRule(body);
		return mainFun(parserData);
	}
	
	return 42;
}

static int params(parseData* parserData){
	int res = 0;
	parserData->parameterIndex = 0;
	if(parserData -> token.Type == tokenIdentifier){
		//fprintf(stderr, "%s\n", parserData->globalTable->Data.identifier);
		
		if (BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value))
		{
			return 43;
		}
		else {
			BSTinsertSymbol(&parserData->localTable, parserData->token.Data.string->value);
			fprintf(stderr, CGRN"L O C A L\n" CWHT);
			Print_tree(parserData->localTable);
		}
		parserData->rID = BSTsearchSymbol(parserData->localTable, parserData->token.Data.string->value);
		addToOutput(codeGenFuncDeclarationOfParam, parserData->rID->identifier, parserData->parameterIndex);
		getToken();
		checkRule(params_n);
	}

	return res;
}

static int params_n(parseData* parserData){
	int res = 0;

	if (parserData -> token.Type == tokenComma){
		getToken();
		checkTokenType(tokenIdentifier);
		parserData->parameterIndex++;
		if (BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value))
		{
			return 43;
		}
		else {
			BSTinsertSymbol(&parserData->localTable, parserData->token.Data.string->value);
			fprintf(stderr, CGRN"L O C A L\n" CWHT);
			Print_tree(parserData->localTable);
		}
		parserData->rID = BSTsearchSymbol(parserData->localTable, parserData->token.Data.string->value);
		addToOutput(codeGenFuncDeclarationOfParam, parserData->rID->identifier, parserData->parameterIndex);
		getToken();
		return(params_n(parserData));
	}

	return res;
}

static int body(parseData* parserData){
	int res = 0;
	//fprintf(stderr,CRED "    <BODY>\n"CWHT);

	if (parserData -> token.Type == tokenIdentifier){

		
		
		getToken();


		if (parserData -> token.Type == tokenAssign){
			getToken();
			checkRule(def_value);
			//getToken();
			checkTokenType2(tokenEndOfLine, tokenEndOfFile);
			getToken();
			return body(parserData);
		}

		else if (parserData -> token.Type == tokenEndOfLine || parserData -> token.Type == tokenEndOfFile){
			/*
			checkTokenType2(tokenEndOfLine, tokenEndOfFile);
			getToken();
			return body(parserData);
			*/
			return SYNTACTICAL;
		}

		else{
			checkRule(func);
			return body(parserData);
		}
	}
	
	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_IF){
		

		//TODO expression in IF statement
		getToken();
		checkRule(expression);
		//getToken();
		checkKeyword(KW_THEN);
		getToken();
		checkTokenType(tokenEndOfLine);
		getToken();
		checkRule(body);
		//getToken();
		checkKeyword(KW_ELSE);
		getToken();
		checkTokenType(tokenEndOfLine);
		getToken();
		checkRule(body);
		//getToken();
		checkKeyword(KW_END);

		getToken();
		checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		getToken();
		return body(parserData);
	}

	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_WHILE){
		

		//TODO expression for do while statement


		getToken();
		checkRule(expression);
		checkKeyword(KW_DO);
		getToken();
		checkTokenType(tokenEndOfLine);
		getToken();
		checkRule(body);
		//getToken();
		checkKeyword(KW_END);
		getToken();
		checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		getToken();
		return body(parserData);
	}
	
	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_PRINT){
		getToken();


		/*
		if (parserData -> token.Type == tokenLeftBracket){	
			getToken();
			checkRule(terms);
			//checkTokenType(tokenRightBracket);
			//getToken();
			checkTokenType2(tokenEndOfLine, tokenEndOfFile);
			return body(parserData);
		}
		*/

			checkRule(terms);
			checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		return body(parserData);

	}
	
	else if(parserData -> token.Type == tokenEndOfLine){
		getToken();
		return body(parserData);
	}
	else if (parserData -> token.Type == tokenString || 
			 parserData -> token.Type == tokenInteger ||
			 parserData -> token.Type == tokenFloat){
		checkRule(expression);
		return body(parserData);
	}


	return SUCCESS;
}



static int terms(parseData* parserData){
	int res;
	
		//getToken();
		checkRule(expression);
		checkRule(terms_n);
	return SUCCESS;
}

static int terms_n(parseData* parserData){
	int res = 0;

	if (parserData -> token.Type == tokenComma){
		getToken();
		checkRule(expression); //docasne ID, nahradit za expression
		//getToken();
		return(terms_n(parserData));
	}

	return res;
}

static int def_value(parseData* parserData){
	int res;
	/*
	if (parserData -> token.Type == tokenIdentifier)
	{
		getToken();
		if (parserData -> token.Type == tokenEndOfLine){
			return SUCCESS;
		}
		else if (parserData -> token.Type == tokenEndOfFile)
		{
			return SUCCESS;
		}
		else{
			checkRule(func);
			return SUCCESS;
		}
	}
	*/

	if (parserData -> token.Type == tokenKeyword)
	{

		switch (parserData -> token.Data.keyword) {
			case KW_INPUTS:
				getToken();
				return SUCCESS;
			case KW_INPUTI:
				getToken();
				return SUCCESS;
			case KW_INPUTF:
				getToken();
				return SUCCESS;
			case KW_LENGTH:
				getToken();
				checkTokenType(tokenLeftBracket);
				getToken();
				if (parserData -> token.Type == tokenIdentifier){
					//zkontrolovat, jestli je dane ID typu string
				}
				else{
					checkTokenType(tokenString);
				}
				getToken();
				checkTokenType(tokenRightBracket);
				getToken();
				return SUCCESS;
			case KW_SUBSTR:
				getToken();
				checkTokenType(tokenLeftBracket);
				getToken();
				if (parserData -> token.Type == tokenIdentifier){
					//zkontrolovat, jestli je dane ID typu string
				}
				else{
					checkTokenType(tokenString);
				}
				getToken();
				checkTokenType(tokenComma);
				getToken();
				if (parserData -> token.Type == tokenIdentifier){
					//zkontrolovat, jestli je dane ID typu string
				}
				else{
					checkTokenType(tokenInteger);
				}
				getToken();
				checkTokenType(tokenComma);
				getToken();
				if (parserData -> token.Type == tokenIdentifier){
					//zkontrolovat, jestli je dane ID typu string
				}
				else{
					checkTokenType(tokenInteger);
				}
				getToken();
				checkTokenType(tokenRightBracket);
				getToken();
				return SUCCESS;
			case KW_ORD:
				getToken();
				checkTokenType(tokenLeftBracket);
				getToken();
				if (parserData -> token.Type == tokenIdentifier){
					//zkontrolovat, jestli je dane ID typu string
				}
				else{
					checkTokenType(tokenString);
				}
				getToken();
				checkTokenType(tokenComma);
				getToken();
				if (parserData -> token.Type == tokenIdentifier){
					//zkontrolovat, jestli je dane ID typu string
				}
				else{
					checkTokenType(tokenInteger);
				}
				getToken();
				checkTokenType(tokenRightBracket);
				getToken();
				return SUCCESS;
			case KW_CHR:
				getToken();
				checkTokenType(tokenLeftBracket);
				getToken();
				if (parserData -> token.Type == tokenIdentifier){
					//zkontrolovat, jestli je dane ID typu string
				}
				else{
					checkTokenType(tokenInteger);
				}
				getToken();
				checkTokenType(tokenRightBracket);
				getToken();
				return SUCCESS;
			default:
				return 42;
		}

	}
	checkRule(expression);
	return SUCCESS;
}

static int args (parseData* parserData){
	int res;
	if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenInteger || 
		parserData -> token.Type == tokenFloat || parserData -> token.Type == tokenString) {
		getToken();
		checkRule(args_n);
		return SUCCESS;
	}
	return 42;
}

static int args_n (parseData* parserData){
	int res = 0;
	//getToken();
	if (parserData -> token.Type == tokenComma){
		getToken();
		checkMultipleTokenType(tokenIdentifier, tokenInteger, tokenFloat, tokenString); 
		getToken();
		return(args_n(parserData));
	}

	return res;
}

static int func (parseData* parserData){
	int res;
	if (parserData -> token.Type == tokenLeftBracket)
			{
				getToken();
				checkRule(args);
				//getToken();
				checkTokenType(tokenRightBracket);
				getToken();
				checkTokenType2(tokenEndOfLine, tokenEndOfFile);
				//getToken();
				return SUCCESS;
			}
			else{
				checkRule(args);
				//getToken();
				checkTokenType2(tokenEndOfLine, tokenEndOfFile);
				//getToken();
				return SUCCESS;
			}
}


void initTables(parseData* parserData){
	BSTInit(&parserData->localTable);
	BSTInit(&parserData->globalTable);

	parserData->currentID = NULL;
	parserData->lID = NULL;
	parserData->rID = NULL;

	parserData->parameterIndex = 0;
	parserData->labelIndex = 0;
	parserData->labelDeep = -1;

	parserData->inFunction = false;
	parserData->inWhileOrIf = false;
}

void freeTables(parseData* parserData){
	BSTdispose(&parserData->localTable);
	BSTdispose(&parserData->globalTable);
}



int setDestFile(FILE *destFile) {
	if((output = destFile) == NULL)
		return SUCCESS;
	return INTERNAL;
	}



int kowalskiAnalysis(){
/*                                                                                                    
                           ``````                           
                      .:+shmNNNNmdhs+-`                     
                   `/shddmNNNNNNNNNNNNms:                   
                 `+hhhddmNNNNNNNNNNNNNNNNh-                 
                -yhhdddmNNNNNNNNNNNNNNNNNNmo`               
               /hhdddmmNNNNNNNNNNNy:::/+ymNNy`              
              /hhddmmmNNNNNNNNNNNo----::::smNh.             
             :hhddmmmNNNNNNNNNNNy-:---:::::/dNh`            
            -hhddmmmNNNNNNNNNNNd::----::::::/yNh`           
           `shddmmmNNNNNNNNmmmms::::::::::::::yNs           
           /ddhhhhdmNNNNNNNmmmh:-::::::::-:::::hN/          
          `ddy----::/oyhddmdhs:..:/+oo+/:::-:::/mm`         
          odd/.---/oyy++oydds-.`-/s+--.-/:--::::+//         
         `/-/----../o..hmy:++-``-/+-hNd-.:--::::::/.        
         .-...---..-o:-ydy/o/.+y+:++shs:++---:::::::        
         -..........ossssso:-ohhds:/oyyhs:---::::::/.       
        `-........``.--:-..:sdhhhdh+----------:+sys/:`      
        :..-+oo+/:--..--:/shdhhyyhddhs++//++oshhhhhy/-      
       `:..+yyyhhhhhhhhhdddhyssoosyhhddddhhhhyyyyhdy//.     
       /-..ohysssyyyyyyyys+/:+oo+osyhhhhhhhhhhdddds////     
       o-.--+ydddhhhhhhyysyyysyyyyhddddddddddddyo/:://+.    
      -o.--..-:+shdddddddddddmmmNNNNmmmmmmdhs+/:::::///s    
      o+.........--/+oyhddmmmNNNNNNNNmdhs+//::::::::://h:   
     .y+..........---:/++ossyhhdmmmmdyo+//:::::::::::::sh`  
     +h+............-:/+++oosyyhddhyso++//:::::::::::::oN:  
    `yd+.............://++++oossyo++///::::::::::::::::/md` 
    +dm+.............///+++++oo+::::::::------:::---::::hNo 
   .hdm+```````.....-///++++++:..---------------------::sNN.
   +ddm+````````````-///++++:.............------------::+NNy
  .hddmo````````````.//+//-.```````............--------:/mNN
 `sdddmy.````````````.--.`````````````..........-------::dNN
 -ddddmd-``````````````````````````````````........-----:dmN
 yddddmm:```````````   `  ``````````````````.......-----:dmN                  
*/
	fprintf(stderr,CGRN"[PARSER]"CWHT" Starting parser.\n"CGRN"[PARSER]"CWHT" Requesting token.\n");
	
	/*
	 * Setting up dynamic string
	*/
	dynString string;
	
	/*
	 * If stringInit returns anything else but 0, 
	 * there was an error while creating string
	*/
	if(stringInit(&string))
		return INTERNAL;
	
	setDynString(&string);

	parseData parserData;

	initTables(&parserData);
	
	//tempString = malloc(sizeof(char)*(strlen(parserData->token.Data.string->value)+1));
	int res;


	/*
	 * Get first token and start parser
	*/
	res = getTokens(&parserData.token);
	
	if (!res){		
		codeGenStart();
		res = mainFun(&parserData);
	}

	stringDispose(&string);
	freeTables(&parserData);

	return res;
}