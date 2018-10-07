#ifndef ERRORS_H
#define ERRORS_H

extern const char *errorTable[];

enum errorMessages{
	SUCCESS = 0,
	LEXICAL = 1,
	SYNTACTICAL = 2,
	SEMANTICAL = 3,
	COMPATIBILITY = 4,
	OTHER = 6,
    ZERO_DIV = 9,
	INTERNAL = 99,
};

/* 
*  @brief Prints error messages to the error stream and returns error type.
*
*  @param   errorID    Type of error
*/
int printError(unsigned int errorID);

#endif