#include <stdio.h>
#include "string.h"
#include "parser.h"
#include "err.h"

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
static int ID(parseData* parserData);

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

#define checkKeyword(_keyword)\
	if(parserData -> token.Type != tokenKeyword\
	   || parserData -> token.Data.keyword != (_keyword)) return SYNTACTICAL





static int mainFun(parseData* parserData){
	printf(CRED "    <MAINFUN>\n"CWHT);
	printf("    %d, %d\n", parserData -> token.Type, parserData -> token.Data.keyword);
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
		
		getToken();
		checkRule(body);

		checkKeyword(KW_END);
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
		checkTokenType2(tokenEndOfLine, tokenEndOfFile);
		return body(parserData);
	}
	
	else if(parserData -> token.Type == tokenKeyword && parserData -> token.Data.keyword == KW_IF){
		

		//TODO expression in IF statement


		getToken();
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
		//TODO: is garbage
		return body(parserData);
	}
	
	else if(parserData -> token.Type == tokenEndOfLine){
		getToken();
		return body(parserData);
	}


	return SUCCESS;
}

static int ID(parseData* parserData){
	if (parserData -> token.Type == tokenAssign){
		/* code */
	}
	return body(parserData);
}

static int print(parseData* parserData){
	int res;

	//empty print statement
	if(parserData -> token.Type == tokenEndOfLine)
		return SUCCESS;

	//TODO

	return SUCCESS;
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
	
	/*
	 * Check if there were any problems with opening source file
	 * or creating dynamic string
	*/
	checkAndSet(&parserData.token);
	
	int res;

	/*
	 * Get first token and start parser
	*/
	getTokens(&parserData.token);
		res = mainFun(&parserData);


	
	if (res)
		return 1;
	
	return SUCCESS;
}