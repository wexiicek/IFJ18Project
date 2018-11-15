#include "parser.h"
#include "string.h"

#define getToken()\
	if(!(res = getTokens(&data -> token)) == SCANNER_TOKEN_OK)\
		return res

#define checkRule(rule)\
	if((res = rule(data)))\
		return res

#define checkTokenType(_type)\
	if(!(data -> token.type == (_type)))\
		return SYNTACTICAL

#define checkKeyword(_keyword)\
	if(data -> token.type != tokenKeyword\
	   || data -> token.value.keyword != (_keyword))\
		return SYNTACTICAL






static int main(parserData* data);
static int body(parserData* data);
static int params(parserData* data);
static int params_n(parserData* data);
static int def_value(parserData* data);
static int print(parserData* data);
static int terms(parserData* data);
static int terms_n(parserData* data);
static int func(parserData* data);
static int func_n(parserData* data);
static int arg(parserData* data);
static int value(parserData* data);

static int main(parserData* data){
	int res;

	//<main> -> DEF ID ( <params> ) EOL <body> END EOL <main>
	/*if(data -> Token.Type == tokenKeyword && data -> token.Data == KW_DEF){
		getToken();
		checkTokenType(tokenIdentifier);
		getToken();
		checkTokenType(tokenLeftBracket);
		getToken();
		checkRule(params);
		//getToken(); ???
		checkTokenType(tokenRightBracket);
		getToken();
		checkTokenType(tokenEndOfLine);
		getToken();
		checkRule(body);
		checkKeyword(KW_END);	

		data -> id -> defined = 1;

		freeSymTable(&data -> localTable); //TODO	

		getToken();
		return main(data);
	}*/

	if(data -> Token.Type == tokenKeyword && data -> token.Data == KW_DEF)
		print("fungujem jak mrdka\n");
	exit(42);
}

static int body(parserData* data){
	
}

static int params(parserData* data){
	
}

static int params_n(parserData* data){
	
}

static int def_value(parserData* data){
	
}

static int print(parserData* data){
	
}

static int terms(parserData* data){
	
}

static int terms_n(parserData* data){
	
}

static int func(parserData* data){
	
}

static int arg(parserData* data){
	
}

static int arg_n(parserData* data){
	
}

static int value(parserData* data){
	
}


int kowalskiAnalyze(){
/*                                                                                                    
            `.:+++/-`             
           :ohmNNNNNNNd+`          
         .yhdmNNNNNNNmNNm:         
        -hddmNNNNNNh:-:/ymo        
       .hdmmNNNNNNd:--:::/mo       
      `ydmmNNNNNmmo:::::::/d/      
      +d+//oyddmdo.:+++::::/m.     
     `oo---o++yso../+o+-:-::/:     
     ...--.:+oyo//y/+yyo:-::::.    
     -......:/:-ohhdo::---:+ss:    
    ..:yyyssosyhhysyhhhyyyhhhd+-   
    /.:yhyyyyyso+oosyhdhhhddy+/+`  
   ./-..:+syddddmmmNNNmdhs+/:::/+  
   +/......-:+oshddmmhs+/:::::::y. 
  `y/.......:/++osyyo+//::::::::oy 
  +d/.``....//++++:------------:/N:
 `hd/``````.//+/:........-------:dd
 oddo```````.-.```````......-----hN
-dddh``````` ````````````.....---hN                      
*/
	printf("jasne sefe\n");
	int res;
	string dynamic;
	
	if(!stringInit(&dynamic))
		return INTERNAL;
	
	parserData parData;
	
	if(!initVars(&parData)){
		stringClear(&dynamic);
		return INTERNAL;
	}
	
	if((res = getTokens(&parData.token)) == SCANNER_TOKEN_OK){//TODO
		if(!codeGenStart()){
			stringClear(&dynamic);
			freeVars(&parData) //TODO
			return INTERNAL;
		}

		res = prog(&parData);
	} 
	
	stringClear(&dynamic);
	freeVars(&parData);

	return SUCCESS;
}