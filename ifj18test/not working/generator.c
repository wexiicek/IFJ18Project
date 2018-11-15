#include <stdio.h>
#include "generator.h"

//Decides which built in function will be used
// 1 - inputs
// 2 - inputi
// 3 - inputf
// 4 - print
// 5 - length
// 6 - substr
// 7 - ord
// 8 - chr
builtInFun = 0;

void codeGenBuiltIn(FILE *dest){
	switch(builtInFun):

		//inputs
		case(1):
		fprintf(dest, "\
			LABEL $inputs\n\
			
			do something
			
			POPFRAME\n\
			RETURN\n");
		break;

		//inputi
		case(2):
		fprintf(dest, "\
			LABEL $inputi\n\

			do something
			
			POPFRAME\n\
			RETURN\n");
		break;

		//inputf
		case(3):
		fprintf(dest, "\
			LABEL $inputf\n\
			
			do something
			
			POPFRAME\n\
			RETURN\n");
		break;

		//print
		case(4):
		fprintf(dest, "\
			LABEL $print\n\
			PUSHFRAME\n\
			
			do something
			
			POPFRAME\n\
			RETURN\n");
		break;

		//length
		case(5):
			fprintf(dest, "\
				LABEL $length\n\
				PUSHFRAME \n\
				DEFVAR LF@%%retval\n\
				STRLEN LF@%%retval LF@0\n\
				POPFRAME\n\
				RETURN\n");
		break;

		//substr
		case(6):
		fprintf(dest, "\
			LABEL $substr\n\
			
			do something
			
			POPFRAME\n\
			RETURN\n");
		break;

		//ord
		//TODO must return NIL if out of range, now returns 0
		case(7):
			fprintf(dest, "\
			LABEL $ord\n\
			PUSHFRAME\n\
			DEFVAR LF@%retval\n\
			DEFVAR LF@lengthCondition\n\
			MOVE LF@retval int@0\n\
			LT LF@lengthCondition LF@%1 int@1\n\
			JUMPIFEQ $asc$return LF@lengthCondition bool@true\n\
			DEFVAR LV@stringlen\n\
			CREATEFRAME\n\
			DEFVAR TF@%0\n\
			MOVE TF@%0 LF@%0\n\
			CALL $length\n\
			MOVE LF@stringlen TF@%retval\n\
			GT LF@lengthCondition LF@%1 LF@stringlen\n\
			JUMPIFEQ $asc$return LF@lengthCondition bool@true\n\
			SUB LF@%1 LF@%1 int@1\n\
			STRI2INT LF@%retval LF@%0 LF@%1\n\
			LABEL $asc$return\n\
			POPFRAME\n\
			RETURN\n");
		break;

		//char
		case(8):
			fprintf(dest, "\
			LABEL $chr\n\
			PUSHFRAME\n\
			DEFVAR LF@%retval\n\
			DEFVAR LF@asciiRng\n\
			MOVE LF@%retval string@\n\
			LT LF@asciiRng LF@%0 int@0\n\
			JUMPIFEQ$cr$return LF@asciiRng bool@true\n\
			GGT LF@asciiRng LF@%0 int@255\n\
			JUMPIFEQ $chr$return LF@asciiRng bool@true\n\
			INT2CHAR LF@%retval LF@%0\n\
			LABEL $chr$return\n\
			POPFRAME\n\
			RETURN\n");
		break;

}