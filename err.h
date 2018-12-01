#ifndef ERR_HEADER
#define ERR_HEADER

/*
 * Defining macros for throwing errors
 * Compiler returns values as follows:
 *   0.. Operation was succesfull
 *	 1.. Lexical problem
 *   2.. Syntactical problem, incorrect syntax
 *   3.. Semantical problem, undefined var or redefinition
 *   4.. Semantical problem, types not compatible
 *   5.. Semantical problem, incorrect count of args while calling a function
 *   6.. Other sementical problems
 *   9.. Division by zero
 *  99.. Internal problems, such as incorrect allocation, etc
*/
#define SUCCESS 0
#define LEXICAL 1 
#define SYNTACTICAL	2
#define SEMANTICAL_UNDEF 3
#define SEMANTICAL_TYPES 4 
#define SEMANTICAL_ARGCOUNT 5
#define SEMANTICAL_OTHER 6
#define ZERO_DIVISION 9
#define INTERNAL 99

#endif 