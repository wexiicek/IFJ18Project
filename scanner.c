/*///////////////////////////////////////////////////////////////////////////////////////
/																						/
/	Subject: IFJ																		/
/	Task: IFJ Project 																	/
/																						/
/	     Author 1 		    															/
/	Dominik Juriga																		/
/		xjurig00																		/
/																						/
/	Team: 045, Variant I 																/
/																						/
///////////////////////////////////////////////////////////////////////////////////////*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "string.h"
#include "scanner.h"
#include "parser.h"
#include "err.h"

/*
 * Macros for colored output
*/
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CWHT  "\x1B[37m"

//Macro definition

//return next character in file
#define nextChar(character) character = (char)getc(code); fprintf(stderr, "stt: %d, chr: %d\n",state, c);

FILE *code;

dynString *kwstring;

//Free the allocated dynamic string and return correct Error Value
static int scanRet(dynString *string, int error) {
	stringDispose(string);
	return error;
}

unsigned int keywordCompare (dynString *str, Token *token) {

	bool kw = false;

	/*
		After finding an identifier token, we check whether it is a keyword or not.
		We check it against all possible keywords using stringCompare function.
	*/

	if (stringCompare(str, "chr"))
	{token -> Data.keyword = KW_CHR; kw = true;}
	else if (stringCompare(str, "def"))
	{token->Data.keyword = KW_DEF; kw = true;}
	else if (stringCompare(str, "do"))
	{token->Data.keyword = KW_DO; kw = true;}
	else if (stringCompare(str, "else"))
	{token->Data.keyword = KW_ELSE; kw = true;}
	else if (stringCompare(str, "end"))
	{token->Data.keyword = KW_END; kw = true;}
	else if (stringCompare(str, "if"))
	{token->Data.keyword = KW_IF; kw = true;}
	else if (stringCompare(str, "inputf"))
	{token->Data.keyword = KW_INPUTF; kw = true;}
	else if (stringCompare(str, "inputi"))
	{token->Data.keyword = KW_INPUTI; kw = true;}
	else if (stringCompare(str, "inputs"))
	{token->Data.keyword = KW_INPUTS; kw = true;}
	else if (stringCompare(str, "length"))
	{token -> Data.keyword = KW_LENGTH; kw = true;}
	else if (stringCompare(str, "not"))
	{token->Data.keyword = KW_NOT; kw = true;}
	else if (stringCompare(str, "nil"))
	{token->Data.keyword = KW_NIL; kw = true;}
	else if (stringCompare(str, "ord"))
	{token->Data.keyword = KW_ORD; kw = true;}
	else if (stringCompare(str, "then"))
	{token->Data.keyword = KW_THEN; kw = true;}
	else if (stringCompare(str, "while"))
	{token->Data.keyword = KW_WHILE; kw = true;}
	else if (stringCompare(str, "print"))
	{token->Data.keyword = KW_PRINT; kw = true;}
	else if (stringCompare(str, "substr"))
	{token->Data.keyword = KW_SUBSTR; kw = true;}

	if (kw) {
		fprintf(stderr, "    KEYWORD: %d, ", token->Data.keyword);
		token->Type = tokenKeyword;
		return true;	//Return true if the identifier is keyword
	}
	return false;		//Return false otherwise
}

int setSourceFile(FILE *sourceFile) {
	/*Set the source code file as a file for scanner.*/
	if ((code = sourceFile) == NULL)
		return SUCCESS;
	return INTERNAL;
}

void setDynString(dynString *string) {
	kwstring = string;
}

/*
	@brief Convert the string value to an integer value and push it to token
	@param from string we want the value from
	@param to token we want to push the value to
	@return SUCCESS
*/
bool strToIntToken(dynString *from, Token *to) {
	int value = atoi(from->value);
	to->Data.integer = value;
	fprintf(stderr, "%d\n", to->Data.integer );
	return SUCCESS;
}

/*
	@brief Convert the string value to a float value and push it to token
	@param from string we want the value from
	@param to token we want to push the value to
	@return SUCCESS
*/
bool strToFltToken(dynString *from, Token *to) {

	float value = strtof(from->value, NULL);
	to->Data.flt = value;
	return SUCCESS;
}

/*
	@brief Convert the string value to a exponential value and push it to token
	@param from string we want the value from
	@param to token we want to push the value to
	@return SUCCESS
*/
bool strToExpToken(dynString *from, Token *to) {
	float value = atof(from->value);
	to->Data.flt = value;
	return SUCCESS;
}

int getTokens (Token *token) {
	/*
		Main scanner function.
		Reads characters from input file - code -.
		Matches them with predefined rules and sets token values and types.
	*/

	//Scanner begins with initialising required variables and checking if
	//there have been no errors with initialising
	if (code == NULL || kwstring == NULL)
		return INTERNAL;

	token->Data.string = kwstring;

	dynString string;
	dynString *str = &string;

	if (stringInit(str))
		return INTERNAL;

	//Check if the first number in input is zero
	bool zeroSwitch = false;
	int state = stateStart;
	token -> Type = tokenEmpty;
	char c;
	//for printing (debugging purposes)
	char *prt = "    TOKEN TYPE:";


	//Checking for block comment at the beginning, when there is no EOL before..
	// .. dont ask
	nextChar(c);
	ungetc(c, code);


	//Go through every character of a file
	while (1) {
		nextChar(c);
		fprintf(stderr, "%s\n", "neco");
		switch (state) {
		case (stateStart):
			//Checking for END OF LINE
			//Or a block comment
			if (c == 10) {
				nextChar(c);
				fprintf(stderr, "%d\n", c);
				if ((c == '=')) {
					state = stateError;
					nextChar(c);
					if (c == 'b') {
						nextChar(c);
						if (c == 'e') {
							nextChar(c);
							if (c == 'g') {
								nextChar(c);
								if (c == 'i') {
									nextChar(c);
									if (c == 'n') {
										nextChar(c);
										//fprintf(stderr, "%c\n",c );
										if (c ==  '\n' || c == 13) {
											while (c != EOF) {
												//Reading characters, while we dont have a sequence
												//of =end OR
												//End of file
												nextChar(c);
												if (c == '\n' || c == '=' || c == 13) {
													nextChar(c);
													if (c == '=' || c == 'e') {
														nextChar(c);
														if ( c == 'e' || c == 'n') {
															nextChar(c);
															if (c == 'n' || c == 'd') {
																nextChar(c);
																if (c == 'd' || c == '\n' || c == EOF || c == 13) {
																	//fprintf(stderr, "%c\n",c );
																	if (c == EOF) {
																		state = stateEnd;
																		token->Type = tokenEndOfFile;
																		break;
																	}
																	else if (c == 'd') {
																		nextChar(c);
																		ungetc(c, code);
																		state = stateStart;
																		token -> Type = tokenEmpty;
																		break;
																	}

																	else if (c == '\n' || c == 13) {
																		state = stateStart;
																		token -> Type = tokenEmpty;
																		break;
																	}
																}
																else return LEXICAL;
																return LEXICAL;
															}
														}
													}
												}
											}
										}
										else if (c == EOF) return LEXICAL;
									}
								}
							}
						}
					}
				}
				//If there was no "=", we return the character and start over
				else {
					ungetc(c, code);
					token -> Type = tokenEndOfLine;
					state = stateEnd;
				}
			}

			else if (isspace(c))
			{state = stateStart; token->Type = tokenEmpty;}

			else if (c == '+')
			{token->Type = tokenAdd; fprintf(stderr, "%s ADD\n", prt); return scanRet(str, SUCCESS);}

			else if (c == '-')
			{token->Type = tokenSub; fprintf(stderr, "%s SUB\n", prt); return scanRet(str, SUCCESS);}

			else if (c == '*')
			{token->Type = tokenMul; fprintf(stderr, "%s MUL\n", prt); return scanRet(str, SUCCESS);}

			else if (c == '/')
			{token->Type = tokenDiv; fprintf(stderr, "%s DIV\n", prt); return scanRet(str, SUCCESS);}

			else if (c == '(')
			{token->Type = tokenLeftBracket; fprintf(stderr, "%s LBRACK\n", prt); return scanRet(str, SUCCESS);}

			else if (c == ')')
			{token->Type = tokenRightBracket; fprintf(stderr, "%s RBRACK\n", prt); return scanRet(str, SUCCESS);}

			else if (c == '{')
			{token->Type = tokenLeftBrace; fprintf(stderr, "%s LBRACE\n", prt); return scanRet(str, SUCCESS);}

			else if (c == '}')
			{token->Type = tokenRightBrace; fprintf(stderr, "%s RBRACE\n", prt); return scanRet(str, SUCCESS);}

			else if (c == ',')
			{token->Type = tokenComma; fprintf(stderr, "%s COMMA\n", prt); return scanRet(str, SUCCESS);}


			/*	Cases, where we CAN'T determine the type definitely	*/

			else if (c == '"')
			{state = stateStringStart; token->Type = tokenString;}

			else if (c == '<')
				state = stateLess;

			else if (c == '>')
				state = stateGreater;

			else if (c == '=')
				state = stateEqual;

			else if (c == '\\')
				state = stateNumberHex;

			else if (isdigit(c)) {
				if (c == 48)
					zeroSwitch = true;

				state = stateNumber; token->Type = tokenInteger;
				ungetc(c, code);
				//if(stringAddChar(str, c)) return scanRet(str, INTERNAL);
			}

			else if (c == '#')
			{state = stateComment;}
			//Question: do we need EOL token on the end of single line
			//			comment?

			else if (c == '!')
				state = stateExclamation;

			else if (c == EOF)
			{token->Type = tokenEndOfFile; state = stateEnd;}

			//This is the beginning of an identifier, it can be either "_" or a lowercase character
			else if ( (c == '_') || (charIsLowercase(c)) )
			{ungetc(c, code); state = stateIdentifierOrKeyword; token->Type = tokenIdentifier;}

			//Otherwise it is an error
			else
			{token->Type = tokenEmpty; return scanRet(str, LEXICAL);}
			break;

		case (stateNumber):
			if ((zeroSwitch == true) && (token->Type == tokenInteger)) {
				fprintf(stderr, "st num:%d\n", c);
				nextChar(c);
				if (isdigit(c)) {
					/*
						if (c == 48) {
							zeroSwitch = false;
							return scanRet(str, LEXICAL);
						}*/
					//Number input in octal
					if (isdigit(c)) {
						do {
							if (c > 47 && c < 56) {
								if (stringAddChar(str, c)) return scanRet(str, INTERNAL);
								nextChar(c);
								fprintf(stderr, "chr%d\n", c);
							}
							else return scanRet(str, LEXICAL);
						} while (isdigit(c));
						//If there is any sign right behind the number, we return LEXICAL
						if (isalpha(c)) return scanRet(str, LEXICAL);
						//otherwise push the value to token
						int tmpval1 = strtol(str->value, NULL, 8);
						token->Data.integer = tmpval1;
						token->Type = tokenInteger;
						state = stateNumberEnd;
						ungetc(c, code);
						break;
					}
				}
				//Number input in binary
				else if (c == 'b') {
					nextChar(c);
					do {
						if (c == 48 || c == 49) {
							//Loading only 0 and 1, otherwise it is an error
							if (stringAddChar(str, c)) return scanRet(str, INTERNAL);
							nextChar(c);
							fprintf(stderr, "%d\n", c);
						}
						else return scanRet(str, LEXICAL);
					} while (isdigit(c));
					//if there is any alphabet character right behind, we return LEXICAL
					if (isalpha(c)) return scanRet(str, LEXICAL);
					//otherwise we push the value to token
					token->Data.integer = strtol(str->value, NULL, 2);
					token->Type = tokenInteger;
					ungetc(c, code);
					state = stateNumberEnd;
					break;
				}
				//Numbers in float
				else if (c == '.') {
					if (stringAddChar(str, c)) return scanRet(str, INTERNAL);
					nextChar(c);
					while (isdigit(c)) {
						if (stringAddChar(str, c)) return scanRet(str, INTERNAL);
						nextChar(c);
					}
					token->Data.flt = strtol(str->value, NULL, 10);
					token->Type = tokenFloat;
					ungetc(c, code);
					state = stateNumberEnd;
					break;

				}
				//char 13 == carriage return, a character behind a zero
				else if (c == ' ' || c == '\n' || c == EOF || c == 13 || c == ')' || c == '(') {

					zeroSwitch = false;
					strToIntToken(str, token);
					state = stateNumberEnd;
					token->Type = tokenInteger;
					fprintf(stderr, "KOKOTKO %d\n", c);
					ungetc(c, code);

				}
				else return scanRet(str, LEXICAL);
			}
			//Decimal and float numbers
			else if (!(zeroSwitch) && (token->Type = tokenInteger)) {
				int fltCnt = 0;
				do {
					if (stringAddChar(str, c)) return scanRet(str, INTERNAL);
					nextChar(c);
					if (c == '.') {
						if (fltCnt == 0) {
							fltCnt = 1;
							//if (!isdigit(c) && fltCnt) return scanRet(str, LEXICAL);
							token->Type = tokenFloat;
							if (stringAddChar(str, c)) return scanRet(str, INTERNAL);
							nextChar(c);
						}
						else return scanRet(str, LEXICAL);
					}
					else if (c == 'e' || c == 'E') {
						token->Type = tokenExponential;
						if (stringAddChar(str, c)) return scanRet(str, INTERNAL);
						nextChar(c);
					}
				} while (isdigit(c));
				//already decimal numbers, putting their values into token
				state = stateNumberEnd;
				if (token->Type == tokenInteger)
					strToIntToken(str, token);
				else if (token->Type == tokenFloat)
					strToFltToken(str, token);
				else if (token->Type == tokenExponential)
					strToExpToken(str, token);
				ungetc(c, code);

			}
			break;

		//BASE.. Hexadecimal number, starting with "\x"
		case (stateNumberHex):
			if (c == 'x') {
				nextChar(c);
				while (isdigit(c) || ((c > 64) && (c < 91)) || ((c > 96) && (c < 123))) {
					if (stringAddChar(str, c)) return scanRet(str, INTERNAL);
					nextChar(c);
				}
				if (!strlen(str->value)) return scanRet(str, LEXICAL);
				//fprintf(stderr, "strln%ld\n", strlen(str->value));
				token->Data.integer = (int)strtol(str->value, NULL, 16);
				ungetc(c, code);
				token->Type = tokenInteger;
				state = stateNumberEnd;
			}
			else return scanRet(str, LEXICAL);
			break;

		case (stateIdentifierOrKeyword):

			if ( (c == '_') || isalnum(c))
			{if (stringAddChar(str, c)) return scanRet(str, INTERNAL);}

			//If we get an ? or !, we add it into identifier and jump to end of identifier..
			//If there are more ? or !, we return LEXICAL error
			else if ( (c == '?') || (c == '!')) {
				if (stringAddChar(str, c))
					return scanRet(str, INTERNAL);
				else
					state = stateIdentifierEnd;
			}

			else if (  (c == ',')  || (c == '=') || isspace(c) || (c == EOF) )
			{ungetc(c, code); state = stateIdentifierEnd;}

			else
			{ungetc(c, code); state = stateIdentifierEnd;}
			break;


		case (stateIdentifierCheck):
			if (isspace(c))
			{ungetc(c, code); state = stateStart;}
			break;

		case (stateEqual):
			//nextChar(c);
			//If "=="
			if (c == '=')
			{token->Type = tokenEqual; state = stateStart;  fprintf(stderr, "%s EQUAL", prt); return scanRet(str, SUCCESS);}
			//If "="
			else
			{ungetc(c, code); token->Type = tokenAssign; state = stateStart;}
			break;

		case (stateLess):
			//If "<="
			if (c == '=')
			{token->Type = tokenLessEqual; state = stateStart;  fprintf(stderr, "%s LESS EQUAL", prt); return scanRet(str, SUCCESS);}
			//If "<"
			else
			{ungetc(c, code); token->Type = tokenLess; state = stateStart;}
			break;

		case (stateGreater):
			//If ">="
			if (c == '=')
			{token->Type = tokenGreaterEqual; state = stateStart;  fprintf(stderr, "%s GREATER EQUAL", prt); return scanRet(str, SUCCESS);}
			//If ">"
			else
			{ungetc(c, code); token->Type = tokenGreater; state = stateStart;}
			break;

		case (stateExclamation):
			//If "!="
			if (c == '=')
			{token->Type = tokenNotEqual; state = stateStart;  fprintf(stderr, "%s NOT EQUAL", prt); return scanRet(str, SUCCESS);}
			//Error otherwise
			else
				return scanRet(str, LEXICAL);
			break;

		case (stateComment):
			//Single line comment
			fprintf(stderr, "cmmnt %d\n", c);
			while (c != EOF || c != 10 || c != 13){
				nextChar(c);
				if (c == 10 || c == 13){
					state = stateStart; token->Type = tokenEndOfLine; ungetc(c, code); break;
				}
				else if (c == EOF)					
					{token->Type = tokenEndOfFile; state = stateEnd; break;}
			}
			break;

		case (stateStringStart):
			token->Type = tokenString;

			if (c < 32)
			{scanRet(str, LEXICAL);}

			//If there is a backslash, means we have to check for escape sequences
			else if (c == '\\')
			{state = stateEscapeSequence;}

			//End string when a second " comes
			else if (c != '\"') {
				if (stringAddChar(str, c))
					return scanRet(str, INTERNAL);
			}
			else
			{state = stateStringEnd;}
			break;

		case (stateEscapeSequence):

			if (token->Type == 24)
				fprintf(stderr, "    TOKEN TYPE: ESCAPE\n");

			// \n escape sequence
			if (c == 'n') {
				c = '\n';
				if (stringAddChar(str, c))
					return scanRet(str, INTERNAL);
				state = stateStringStart;
			}

			// \t escape sequence
			else if (c == 't') {
				c = '\t';
				if (stringAddChar(str, c))
					return scanRet(str, INTERNAL);
				state = stateStringStart;
			}

			// \s escape sequence
			else if (c == 's') {
				c = ' ';
				if (stringAddChar(str, c))
					return scanRet(str, INTERNAL);
				state = stateStringStart;
			}

			// \" escape sequence
			else if (c == '"') {
				c = '"';
				if (stringAddChar(str, c))
					return scanRet(str, INTERNAL);
				state = stateStringStart;
			}

			// \\ escape sequence
			else if (c == '\\') {
				c = '\\';
				if (stringAddChar(str, c))
					return scanRet(str, INTERNAL);
				state = stateStringStart;
			}

			// hexadecimal escape sequence
			else if (c == 'x') {
				/*
				 if we have \xNM, where NM is a hexadecimal number,
				 we save it into a temporary string which will consist of
				 [0][1]            [2][3]				[4]
				  0  x 				N  M   				\0
				 hex num start 		our hex number 		string closing symbol
				 Then we convert it to decimal value and concatenate to
				 the string, if it has correct values (between 0 and FF (255) ASCII)
				*/
				char temp[5];
				temp[0] = '0'; temp[1] = 'x'; temp[4] = '\0';
				nextChar(c);
				if (isalnum(c))	temp[2] = c;
				else return LEXICAL;
				nextChar(c);

				if (c == ' ') {
					state = stateStringStart;
					break;
				}

				else if (isalnum(c))	temp[3] = c;
				else return LEXICAL;

				long decval = strtol(temp, NULL, 16);

				if (decval < 0 || decval > 255)
					return LEXICAL;

				else {
					if (stringAddChar(str, (char) decval))
						return scanRet(str, INTERNAL);
				}
				state = stateStringStart;
			}

			else {
				if (stringAddChar(str, c))
					return scanRet(str, INTERNAL);
				state = stateStringStart;
			}

			break;

		case (stateStringEnd):
			if (!stringCompare(str, "\n") == 0)
				fprintf(stderr, "    TOKEN TYPE: STRING | TOKEN VAL: EOL\n");
			else
				fprintf(stderr, "    TOKEN TYPE: %d | TOKEN VAL: \"%s\"\n", token->Type , str->value);
			state = stateStart;
			if (!pushToToken(str, kwstring)) return scanRet(str, INTERNAL);
			ungetc(c, code);
			return scanRet(str, SUCCESS);//TODO CHeck if its SUCCESS
			break;

		case (stateIdentifierEnd):

			//Check if the current identifier is keyword
			if (keywordCompare(str, token)) {
				state = stateStart;
				fprintf(stderr, "    TOKEN TYPE: %d | TOKEN VAL: \"%s\"\n", token->Type , str->value);
				ungetc(c, code);
				return scanRet(str, SUCCESS);
			}

			//If it is not keyword, we push the identifier value into token
			else {
				if (!stringCompare(str, "\n") == 0)
					fprintf(stderr, "    TOKEN TYPE: ID | TOKEN VAL: EOL\n");
				else
					fprintf(stderr, "    TOKEN TYPE: %d | TOKEN VAL: \"%s\"\n", token->Type , str->value);
				state = stateStart;
				if (!pushToToken(str, token->Data.string)) return scanRet(str, INTERNAL);

				ungetc(c, code);
				return scanRet(str, SUCCESS);
			}
			break;

		case (stateNumberEnd):
			//FLOAT
			if (token->Type == 11) {
				strToFltToken(str, token);
				fprintf(stderr, "    TOKEN TYPE: FLOAT | TOKEN VAL: %f\n", token->Data.flt);
			}
			//EXPONENTIAL
			else if (token->Type == 12) {

				fprintf(stderr, "    TOKEN TYPE: EXP | TOKEN VAL: %s\n", token->Data.string->value);
			}
			//INTEGER
			else if (token->Type == 22) {
				fprintf(stderr, "    TOKEN TYPE: INT | TOKEN VAL:  %d\n", token->Data.integer);
			}
			state = stateStart;
			ungetc(c, code);
			return scanRet(str, SUCCESS);

			break;

		case (stateError):
			return scanRet(str, LEXICAL);
			break;

		case (stateEnd):
			return scanRet(str, SUCCESS);
			break;
		}

		/*
			Prints for debugging purposes.
		*/
		char * tType = "    TOKEN TYPE:";
		switch (token->Type) {

		case 10:
			fprintf(stderr, "%s EOF\n", tType);
			return scanRet(str, SUCCESS);
			break;
		case 13:
			fprintf(stderr, "%s EQUAL\n", tType);
			return scanRet(str, SUCCESS);
			break;
		case 14:
			fprintf(stderr, "%s NOT EQUAL\n", tType);
			return scanRet(str, SUCCESS);
			break;
		case 15:
			fprintf(stderr, "%s LESS\n", tType);
			return scanRet(str, SUCCESS);
			break;
		case 16:
			fprintf(stderr, "%s GREATER\n", tType);
			return scanRet(str, SUCCESS);
			break;
		case 17:
			fprintf(stderr, "%s LESS EQUAL\n", tType);
			return scanRet(str, SUCCESS);
			break;
		case 18:
			fprintf(stderr, "%s GREATER EQUAL\n", tType);
			return scanRet(str, SUCCESS);
			break;
		case 19:
			fprintf(stderr, "%s EOL\n", tType);
			return scanRet(str, SUCCESS);
			//return SUCCESS;
			break;
		case 20:
			fprintf(stderr, "%s NUMBER\n", tType);
			return scanRet(str, SUCCESS);
			break;
		case 21:
			fprintf(stderr, "%s ASSIGN\n", tType);
			return scanRet(str, SUCCESS);
			//return SUCCESS;
			break;
		default:
			break;
		}
	}
}