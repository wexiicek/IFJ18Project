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
	parserData->prevToken = parserData->token;\
	res =getTokens(&parserData->token);\
	if(res) return res;

#define checkRule(rule)\
	if((res = rule(parserData))) return res

#define checkRulePrint(rule)\
	rule(parserData)

#define checkTokenType(_type)\
	if(!(parserData -> token.Type == (_type))) return SYNTACTICAL

#define checkPrevTokenType(_type)\
	if (!(parserData -> prevToken.Type == (_type))) return SYNTACTICAL

#define checkTokenType2(_type, _type2)\
	if (parserData -> token.Type == (_type2)) return 0;\
	else if (!(parserData -> token.Type == (_type))) return SYNTACTICAL

#define checkMultipleTokenType(_type, _type2, _type3, _type4)\
		if (!(parserData -> token.Type == (_type)) && !(parserData -> token.Type == (_type2)) &&\
		    !(parserData -> token.Type == (_type3)) && !(parserData -> token.Type == (_type4))) return SEMANTICAL_OTHER

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
		parserData -> inFunction = true;
		parserData -> labelIndex = 0;
		getToken();
		//fprintf(stderr, "print %d\n", Print_tree(parserData->globalTable));
		//fprintf(stderr, "	we have		%s %d %d\n", parserData->token.Data.string->value, parserData->token.Type, parserData->token.Data.string->length);
		checkTokenType(tokenIdentifier);

		if (BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value) == NULL)
		{

			BSTinsertSymbol(&parserData->globalTable, parserData->token.Data.string->value);

		}
		else{
			return SEMANTICAL_UNDEF;
		}

		parserData->currentID = BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value);
		//fprintf(stderr, "%s\n", parserData->currentID->identifier);
		addToOutput(codeGenFuncBegin, parserData->currentID->identifier);
		

		//Print_tree(parserData->globalTable);
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
		fprintf(stderr, "CurrTkn:"CGRN" %d"CWHT".. PrevTkn:"CGRN"%s\n"CWHT, parserData->token.Data.keyword, parserData->prevToken.Data.string->value );
		checkKeyword(KW_END);
		addToOutput(codeGenFuncEnd, parserData->currentID->identifier);
		BSTdispose(&parserData->localTable);
		getToken();
		checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		getToken();
		parserData -> inFunction = false;
		return (mainFun(parserData));
	}

	//<main> -> EOL <main>
	else if (parserData -> token.Type == tokenEndOfLine){
		getToken();

		return mainFun(parserData);
	}

	//<main> -> EOF
	else if (parserData -> token.Type == tokenEndOfFile){
		return SUCCESS;
	}

	//<main> -> <body> <main>
	else if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenEndOfLine || parserData -> token.Data.keyword == KW_IF || parserData -> token.Data.keyword == KW_WHILE || parserData -> token.Data.keyword == KW_PRINT){
		parserData -> labelIndex = 0;
		checkRule(body);
		return mainFun(parserData);
	}
	
	return SYNTACTICAL;
}

static int params(parseData* parserData){
	int res = 0;
	parserData->parameterIndex = 0;

	//<params> -> ID <params_n>
	if(parserData -> token.Type == tokenIdentifier){

		//fprintf(stderr, "%s\n", parserData->globalTable->Data.identifier);
		/*
		if (BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value))
		{
			return 43; // TODO aky err?
		}
		else {
		}
		*/

		parserData->currentID->argCounter = 0;
		BSTinsertSymbol(&parserData->localTable, parserData->token.Data.string->value);
		parserData->currentID->argCounter+=1;
		fprintf(stderr, CGRN"L O C A L\n" CWHT);
		//Print_tree(parserData->localTable);
		parserData->rID = BSTsearchSymbol(parserData->localTable, parserData->token.Data.string->value);
		parserData->rID->global = false;
		//Print_tree(parserData->localTable);
		addToOutput(codeGenFuncDeclarationOfParam, parserData->rID->identifier, parserData->parameterIndex);
		getToken();
		checkRule(params_n);
	}
	//<params> -> E
	return res;
}

static int params_n(parseData* parserData){
	int res = 0;

	//<params_n> -> , ID <params_n>
	if (parserData -> token.Type == tokenComma){
		getToken();
		checkTokenType(tokenIdentifier);
		parserData->parameterIndex++;
		parserData->currentID->argCounter+=1;

		/*
		if (BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value))
		{
			return 43;
		}
		else {
		}
		*/

		BSTinsertSymbol(&parserData->localTable, parserData->token.Data.string->value);
		fprintf(stderr, CGRN"L O C A L\n" CWHT);
		//Print_tree(parserData->localTable); 	
		parserData->rID = BSTsearchSymbol(parserData->localTable, parserData->token.Data.string->value);
		parserData->rID->global = false;
		//Print_tree(parserData->localTable);
		addToOutput(codeGenFuncDeclarationOfParam, parserData->rID->identifier, parserData->parameterIndex);
		getToken();
		return(params_n(parserData));
	}
	fprintf(stderr, "ARG COUNTER: %d\n", parserData->currentID->argCounter);
	//<params_n> -> E
	return res;
}

static int body(parseData* parserData){
	int res = 0;
	//fprintf(stderr,CRED "    <BODY>\n"CWHT);
	//<body> -> ID <id> EOL <body>
	if (parserData -> token.Type == tokenIdentifier){

			tData *tempCurrentID = BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value);
			if(tempCurrentID != NULL){
				parserData->currentID = BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value); 
				parserData->currentID->callArgCounter = 0;
			}
			getToken();

			//<id> -> = <def_value>
			if (parserData -> token.Type == tokenAssign){
				if (parserData -> inFunction == true) {
					if ((BSTsearchSymbol(parserData -> localTable, parserData -> prevToken.Data.string->value)) == NULL) {
							BSTinsertSymbol(&parserData -> localTable, parserData -> prevToken.Data.string->value);
							parserData -> lID = BSTsearchSymbol(parserData -> localTable, parserData -> prevToken.Data.string->value);
							addToOutput(codeGenDeclarationOfVar, parserData -> lID -> identifier, parserData -> lID -> global);
						}
				}
				else {
					if ((BSTsearchSymbol(parserData -> globalTable, parserData -> prevToken.Data.string->value)) == NULL) {
							BSTinsertSymbol(&parserData -> globalTable, parserData -> prevToken.Data.string->value);
							parserData -> lID = BSTsearchSymbol(parserData -> globalTable, parserData -> prevToken.Data.string->value);
							parserData -> lID -> global = true;
							addToOutput(codeGenDeclarationOfVar, parserData -> lID -> identifier, parserData -> lID -> global);
					}
				}
				//fprintf(stderr, "%s\n", "GLOBALNI TABULKA:");
				//Print_tree(parserData -> globalTable);
				//fprintf(stderr, "%s\n", "LOKALNI TABULKA:");
				//Print_tree(parserData -> localTable);
				
				getToken();
				checkRule(def_value);
				checkTokenType2(tokenEndOfLine, tokenEndOfFile);
				getToken();
				return body(parserData);
			}
			//<id> -> E
			else if (parserData -> token.Type == tokenEndOfLine || parserData -> token.Type == tokenEndOfFile){
				if (parserData->currentID->argCounter == 0) {
					checkTokenType2(tokenEndOfLine, tokenEndOfFile);
					getToken();
					return body(parserData);
				}
				return SEMANTICAL_ARGCOUNT;
			}
			//<id> -> <func>
			else{
				if (parserData->currentID != NULL){
					checkRule(func);
					addToOutput(codeGenFuncCall, parserData->currentID->identifier);
					return body(parserData);
				}
				else return SEMANTICAL_UNDEF;
			}
		}

	//<body> -> IF <expession> THEN EOL <body> ELSE EOL <body> END EOL <body>
	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_IF){
		parserData->labelDeep++;
		parserData->inWhileOrIf = true;

		parserData->lID = BSTsearchSymbol(parserData->globalTable, "%result");
		if (parserData->inFunction == true)
		{
			parserData->lID->global = false;
		}
		else {
			parserData->lID->global = true;
		}
		int currentLabelIndex = parserData -> labelIndex;
		char *functionID = parserData -> currentID ? parserData -> currentID -> identifier : "";
		parserData -> labelIndex += 2;
		//TODO expression in IF statement
		getToken();
		checkRule(expression);
		//getToken();
		checkKeyword(KW_THEN);
		getToken();
		checkTokenType(tokenEndOfLine);

		addToOutput(codeGenIfBegin, functionID, currentLabelIndex, parserData->labelDeep);

		getToken();
		checkRule(body);
		//getToken();
		checkKeyword(KW_ELSE);
		getToken();
		checkTokenType(tokenEndOfLine);

		addToOutput(codeGenIfElse, functionID, currentLabelIndex, parserData->labelDeep);

		getToken();
		checkRule(body);
		//getToken();
		checkKeyword(KW_END);

		addToOutput(codeGenIfEnd, functionID, currentLabelIndex+1, parserData->labelDeep);

		parserData->labelDeep--;
		parserData->inWhileOrIf = false;

		getToken();
		checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		getToken();
		return body(parserData);
	}

	//<body> -> WHILE <expression> DO EOL <body> END EOL <body>
	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_WHILE){
		

		//TODO expression for do while statement
		parserData->labelDeep++;
		parserData->inWhileOrIf = true;

		parserData->lID = BSTsearchSymbol(parserData->globalTable, "%result");

		int currentLabelIndex = parserData -> labelIndex;
		char *functionID = parserData -> currentID ? parserData -> currentID -> identifier : "";
		parserData -> labelIndex += 2;
		addToOutput(codeGenLabel, functionID, currentLabelIndex, parserData->labelDeep);

		getToken();
		checkRule(expression);
		addToOutput(codeGenWhileBegin, functionID, currentLabelIndex+1, parserData->labelDeep);
		checkKeyword(KW_DO);
		getToken();
		checkTokenType(tokenEndOfLine);
		getToken();
		checkRule(body);
		//getToken();
		checkKeyword(KW_END);

		addToOutput(codeGenWhileEnd, functionID, currentLabelIndex+1, parserData->labelDeep);

		parserData->labelDeep--;
		parserData->inWhileOrIf = false;


		getToken();
		checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		getToken();
		return body(parserData);
	}

	//<body> -> PRINT ( <terms> ) EOL <body>
	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_PRINT){
		getToken();


		parserData->lID = BSTsearchSymbol(parserData->globalTable, "%result");
		parserData->lID->dataType = TYPE_UNDEFINED;
		if (parserData->inFunction == true)
		{
			parserData->lID->global = false;
		}
		else
			parserData->lID->global = true;
		if (parserData -> token.Type == tokenLeftBracket){	
			getToken();
			fprintf(stderr, "%s\n", "kkt");
			res = checkRulePrint(terms);
			fprintf(stderr, "%s %d\n","res ve funkci print",res);
			checkTokenType(tokenRightBracket);
			getToken();
			checkTokenType2(tokenEndOfLine, tokenEndOfFile);
			return body(parserData);
		}
			
			checkRule(terms);

			checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		return body(parserData);

	}

	//<body> -> E
	else if(parserData -> token.Type == tokenEndOfLine){
		getToken();
		return body(parserData);
	}

	//<body> -> <expression> <body>
	else if (parserData -> token.Type == tokenString || 
			 parserData -> token.Type == tokenInteger ||
			 parserData -> token.Type == tokenFloat){
		checkRule(expression);
		return body(parserData);
	}


	return SUCCESS;
}


//<terms> -> <expression> <terms_n>
static int terms(parseData* parserData){
	int res;
	
/*
		if (parserData->token.Type == tokenIdentifier)
		{
			if (parserData->inFunction == true) {
				parserData->lID = BSTsearchSymbol(parserData->localTable, parserData->token.Data.string->value);
				if (parserData->lID == NULL)	
					return SEMANTICAL_UNDEF;
				else
					parserData->lID->global = false;
			}
			else {
				parserData->lID = BSTsearchSymbol(parserData->globalTable, parserData->token.Data.string->value);
				if (parserData->lID == NULL)
					return SEMANTICAL_UNDEF;
				else
					parserData->lID->global = true;}
		}
*/
		res = checkRulePrint(expression);
		fprintf(stderr, "%s %d\n","res ve funkci terms",res);
		if (res == 2 && parserData->token.Type == tokenRightBracket)
		{
			res = 0;
			addToOutput(codeGenPrintBracket,);
			return res;
		}
		fprintf(stderr, "%s %d\n","res ve funkci terms",res);
		addToOutput(codeGenPrint, *parserData);
		checkRule(terms_n);
	return res;
}

static int terms_n(parseData* parserData){
	int res = 0;

	//<terms_n> -> , <expression> <terms_n>
	if (parserData -> token.Type == tokenComma){
		getToken();

		res = checkRulePrint(expression);
		fprintf(stderr, "%s %d\n","res ve funkci terms_n",res); //docasne ID, nahradit za expression
		if (res == 2 && parserData->token.Type == tokenRightBracket)
		{
			res = 0;
			addToOutput(codeGenPrintBracket,);
			return res;
		}
		addToOutput(codeGenPrint,);
		//getToken();
		return(terms_n(parserData));
	}

	//<terms_n> -> E
	fprintf(stderr, "%s %d\n","res ve funkci terms_n pred returnem",res);
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

			//<def_value> -> INPUTS || INPUTS()
			case KW_INPUTS:
				getToken();
				if (parserData->token.Type == tokenLeftBracket)
				{
					getToken();
					checkTokenType(tokenRightBracket);
					getToken();
					addToOutput(codeGenFuncCall, "inputs");
					addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
					return SUCCESS;
				}
				addToOutput(codeGenFuncCall, "inputs");
				addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
				return SUCCESS;

			//<def_value> -> INPUTI || INPUTI()
			case KW_INPUTI:
				getToken();
				if (parserData->token.Type == tokenLeftBracket)
				{
					getToken();
					checkTokenType(tokenRightBracket);
					getToken();
					addToOutput(codeGenFuncCall, "inputi");
					addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
					return SUCCESS;
				}
				addToOutput(codeGenFuncCall, "inputi");
				addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
				return SUCCESS;

			//<def_value> -> INPUTF || INPUTF()
			case KW_INPUTF:
				getToken();
				if (parserData->token.Type == tokenLeftBracket)
				{
					getToken();
					checkTokenType(tokenRightBracket);
					getToken();
					addToOutput(codeGenFuncCall, "inputf");
					addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
					return SUCCESS;
				}
				addToOutput(codeGenFuncCall, "inputf");
				addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
				return SUCCESS;

			//<def_value> -> LENGHT ( ID || STRING_VALUE )
			case KW_LENGTH:
				addToOutput(codeGenFuncBeforeEnterParam,);
				getToken();
				checkTokenType(tokenLeftBracket);
				getToken();
				if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenString){
				addToOutput(codeGenFuncEnterParam, *parserData, 0);
				getToken();
				checkTokenType(tokenRightBracket);
				addToOutput(codeGenFuncCall, "length");
				addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
				getToken();
				return SUCCESS;
				}
				else
					return SEMANTICAL_TYPES;

			//<def_value> -> SUBSTR ( ID || STRING_VALUE, ID || INT_VALUE, ID || INT_VALUE)
			case KW_SUBSTR:
				addToOutput(codeGenFuncBeforeEnterParam,);
				getToken();
				checkTokenType(tokenLeftBracket);
				getToken();
				if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenString){
					addToOutput(codeGenFuncEnterParam, *parserData, 0);
					getToken();
					checkTokenType(tokenComma);
					getToken();
				}
				else{
					return SEMANTICAL_TYPES;
				}
				
				if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenInteger){
					addToOutput(codeGenFuncEnterParam, *parserData, 1);
					getToken();
					checkTokenType(tokenComma);
					getToken();
				}
				else{
					return SEMANTICAL_TYPES;
				}
				if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenInteger){
					addToOutput(codeGenFuncEnterParam, *parserData, 2);
					getToken();
					checkTokenType(tokenRightBracket);
					addToOutput(codeGenFuncCall, "substr");
					addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
					getToken();
				}
				else{
					return SEMANTICAL_TYPES;
				}
				return SUCCESS;

			//<def_value> -> ORD (ID || STRING_VALUE, ID || INT_VALUE)
			case KW_ORD:
				addToOutput(codeGenFuncBeforeEnterParam,);
				getToken();
				checkTokenType(tokenLeftBracket);
				getToken();
				if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenString){
					addToOutput(codeGenFuncEnterParam, *parserData, 0);
					getToken();
					checkTokenType(tokenComma);
					getToken();
				}
				else{
					return SEMANTICAL_TYPES;
				}
				if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenInteger){
					addToOutput(codeGenFuncEnterParam, *parserData, 1);
					getToken();
					checkTokenType(tokenRightBracket);
					addToOutput(codeGenFuncCall, "ord");
					addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
					getToken();
				}
				else{
					return SEMANTICAL_TYPES;
				}
				return SUCCESS;

			//<def_value> -> CHR (ID || INT_VALUE)
			case KW_CHR:
				addToOutput(codeGenFuncBeforeEnterParam,);
				getToken();
				checkTokenType(tokenLeftBracket);
				getToken();
				if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenInteger){
					addToOutput(codeGenFuncEnterParam, *parserData, 0);
					getToken();
					checkTokenType(tokenRightBracket);
					addToOutput(codeGenFuncCall, "chr");
					addToOutput(codeGenFuncReturnValueAssign, parserData->lID->identifier, *parserData);
					getToken();
				}
				else{
					return SEMANTICAL_TYPES;
				}
				return SUCCESS;
			default:
				return SYNTACTICAL;
		}

	}
	//<def_value> -> <expression>
	checkRule(expression);
	//Print_tree(parserData->localTable);
	return SUCCESS;
}

//<arg> -> <value> <arg_n>
static int args (parseData* parserData){
	int res;

	//<value> -> INT_VALUE || FLOAT_VALUE || STRING_VALUE || ID
	if (parserData -> token.Type == tokenIdentifier || parserData -> token.Type == tokenInteger || 
		parserData -> token.Type == tokenFloat || parserData -> token.Type == tokenString) {
		addToOutput(codeGenFuncEnterParam, *parserData, parserData->currentID->callArgCounter);
		parserData->currentID->callArgCounter += 1;

		getToken();
		checkRule(args_n);
		return SUCCESS;
	}
	else if (parserData->currentID->argCounter == 0)
		return SUCCESS;
	return SEMANTICAL_ARGCOUNT;
}

//<arg_n> -> , <value> <arg_n>
static int args_n (parseData* parserData){
	int res = 0;
	if (parserData -> token.Type == tokenComma){
		
		getToken();

		//<value> -> INT_VALUE || FLOAT_VALUE || STRING_VALUE || ID
		checkMultipleTokenType(tokenIdentifier, tokenInteger, tokenFloat, tokenString);
		addToOutput(codeGenFuncEnterParam, *parserData, parserData->currentID->callArgCounter);
		parserData->currentID->callArgCounter += 1;
		getToken();
		return(args_n(parserData));
	}

	return res;
}

static int func (parseData* parserData){
	int res;
	addToOutput(codeGenFuncBeforeEnterParam,);
	//<func> -> ( <arg> )
	if (parserData -> token.Type == tokenLeftBracket){
				getToken();
				checkRule(args);
				checkTokenType(tokenRightBracket);
				fprintf(stderr, "FN ARGS:%d - FN CALL ARGS:%d\n",parserData->currentID->argCounter, parserData->currentID->callArgCounter );
				if (parserData->currentID->argCounter != parserData->currentID->callArgCounter)
					return SEMANTICAL_ARGCOUNT;
				getToken();
				checkTokenType2(tokenEndOfLine, tokenEndOfFile);
				return SUCCESS;
			}
	//<func> -> <arg>
	else{
		//fprintf(stderr, "%d\n", parserData->currentID->argCounter);

		checkRule(args);
		if (parserData->currentID->argCounter != parserData->currentID->callArgCounter)
			return SEMANTICAL_ARGCOUNT;

		fprintf(stderr, "FN ARGS:%d - FN CALL ARGS:%d\n",parserData->currentID->argCounter, parserData->currentID->callArgCounter );
		checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		return SUCCESS;
	}
}


void initTables(parseData* parserData){
	tData *id;
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

	BSTinsertSymbol(&parserData->globalTable, "%result");
	id = BSTsearchSymbol(parserData->globalTable, "%result");
	id->defined = true;
	id->dataType = TYPE_UNDEFINED;
	id->global = true;
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
		addToOutput(codeGenStart,);
		addToOutput(codeGenMainFrameBegin,);
		res = mainFun(&parserData);
	}
	addToOutput(codeGenMainFrameEnd,);
	stringDispose(&string);
	freeTables(&parserData);

	return res;
}