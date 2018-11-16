#include <stdio.h>
#include "string.h"
#include "parser.h"

#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"

static int params(parseData *parserData);
static int params_n(parseData* parserData);
static int body(parseData* parserData);
static int ID(parseData* parserData);

#define getToken()\
	getTokens(&parserData->token)

#define checkRule(rule)\
	if((res = rule(parserData))) return res

#define checkTokenType(_type)\
	if(!(parserData -> token.Type == (_type))) return 3

#define checkKeyword(_keyword)\
	if(parserData -> token.Type != tokenKeyword\
	   || parserData -> token.Data.keyword != (_keyword)) return 3





static int mainFun(parseData* parserData){
	printf("	%d, %d\n", parserData -> token.Type, parserData -> token.Data.keyword);
	int res;
	//<main> -> DEF ID ( <params> ) EOL <body> END EOL <main>
	if((parserData -> token.Type == tokenKeyword) && (parserData->token.Data.keyword == KW_DEF)){
		//Checking function definition
		getToken();
		checkTokenType(tokenIdentifier);
		getToken();
		checkTokenType(tokenLeftBracket);
		getToken();
		
		//Check parameter syntax
		checkRule(params);
		
		//Check body of the function
		checkTokenType(tokenRightBracket);

		getToken();
		checkTokenType(tokenEndOfLine);
		
		checkRule(body);

		checkKeyword(KW_END);
		getToken();
		checkTokenType(tokenEndOfLine);
		getToken();

		return (mainFun(parserData));
	}

	else if (parserData -> token.Type == tokenEndOfLine){
		getToken();
		return mainFun(parserData);
	}

	else if (parserData -> token.Type == tokenEndOfFile){
		return 0;
	}
	else return body(parserData);
}

static int params(parseData* parserData){
	int res;
	if(parserData -> token.Type == tokenIdentifier){
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
		getToken();
		return(params_n(parserData));
	}

	return res;
}

static int body(parseData* parserData){
	int res = 0;
	printf(CRED "    <BODY>\n"CWHT);

	if (parserData -> token.Type == tokenIdentifier){
		getToken();
		checkRule(ID);
		getToken();
		checkTokenType(tokenEndOfLine);
		return body(parserData);
	}
	
	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_IF){
		

		//TODO expression in IF statement


		getToken();
		checkKeyword(KW_THEN);
		getToken();
		checkTokenType(tokenEndOfLine);
		checkRule(body);
		//getToken();
		checkKeyword(KW_ELSE);
		getToken();
		checkTokenType(tokenEndOfLine);
		checkRule(body);
		//getToken();
		checkKeyword(KW_END);
		getToken();
		checkTokenType(tokenEndOfLine);
		getToken();
		return body(parserData);
	}

	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_WHILE){
		

		//TODO expression for do while statement


		getToken();
		checkKeyword(KW_DO);
		getToken();
		checkTokenType(tokenEndOfLine);
		checkRule(body);
		getToken();
		checkKeyword(KW_END);
		getToken();
		checkTokenType(tokenEndOfLine);
		getToken();
		return body(parserData);
	}
	
	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_PRINT){
		//TODO: is garbage
		return body(parserData);
	}
	
	else if(parserData -> token.Type == tokenEndOfLine){
		getToken();
		return body(parserData);
	}

	else {
		return 0;
	}
}

static int ID(parseData* parserData){
	return body(parserData);
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
	printf(CGRN"[PARSER]"CWHT" Starting parser.\n"CGRN"[PARSER]"CWHT" Requesting token.\n");
	dynString string;
	if(stringInit(&string))
		return INTERNAL;
	setDynString(&string);

	parseData parserData;
	checkAndSet(&parserData.token);
	
	int res;

	if(getTokens(&parserData.token))
		res = mainFun(&parserData);
	if (res)
		return 1;
	return 0;
}