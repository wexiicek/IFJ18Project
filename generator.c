#include <stdio.h>
#include <ctype.h>
#include "generator.h"
#include "symtable.h"
#include "string.h"
#include "expression.h"


FILE *test;
//Decides which built in function will be used
// 1 - inputs
// 2 - inputi
// 3 - inputf
// 4 - print
// 5 - length
// 6 - substr
// 7 - ord
// 8 - chr
//int builtInFun = 0;

void codeGenBuiltIn(FILE *dest, int builtInFun){
	switch(builtInFun) {

		//inputs
		case(1):
		fprintf(dest, 
"\
LABEL $inputs\n\
PUSHFRAME\n\
DEFVAR LF@%%retval\n\
READ LF@%%retval string\n\
POPFRAME\n\
RETURN\n");
		break;

		//inputi
		case(2):
		fprintf(dest, \
"\
LABEL $inputi\n\
PUSHFRAME\n\
DEFVAR LF@%%retval\n\
READ LF@%%retval int\n\
POPFRAME\n\
RETURN\n");
		break;

		//inputf
		case(3):
		fprintf(dest, \
"\
LABEL $inputf\n\
PUSHFRAME\n\
DEFVAR LF@%%retval\n\
READ LF@%%retval float\n\
POPFRAME\n\
RETURN\n");
		break;

		//print
		case(4):
		fprintf(dest, \
"\
LABEL $print\n\
PUSHFRAME\n\
DEFVAR LF@%%retval\n\
WRITE LF@%%retval\n\
POPFRAME\n\
RETURN\n");
		break;

		//length
		case(5):
			fprintf(dest, \
"\
LABEL $length\n\
PUSHFRAME \n\
DEFVAR LF@%%retval\n\
STRLEN LF@%%retval LF@%%0\n\
POPFRAME\n\
RETURN\n");
		break;

		//substr
		case(6):
		fprintf(dest, \
"\
LABEL $substr\n\
PUSHFRAME\n\
DEFVAR LF@%%retval\n\
MOVE LF@%%retval string@\n\
DEFVAR LF@length\n\
STRLEN LF@length LF@%%0\n\
DEFVAR LF@condition\n\
LT LF@condition LF@%%1 int@0\n\
JUMPIFEQ $return$nil LF@condition bool@true\n\
GT LF@condition LF@%%1 LF@length\n\
JUMPIFEQ $return$nil LF@condition bool@true\n\
LT LF@condition LF@%%2 int@0\n\
JUMPIFEQ $return$nil LF@condition bool@true\n\
DEFVAR LF@index\n\
MOVE LF@index LF@%%1\n\
DEFVAR LF@char\n\
MOVE LF@char string@\n\
DEFVAR LF@substrEnd\n\
ADD LF@substrEnd LF@index LF@%%2\n\
DEFVAR LF@tmpCond\n\
LABEL $while\n\
GETCHAR LF@char LF@%%0 LF@index\n\
CONCAT LF@%%retval LF@%%retval LF@char\n\
ADD LF@index LF@index int@1\n\
LT LF@condition LF@index LF@substrEnd\n\
LT LF@tmpCond LF@index LF@length\n\
AND LF@condition LF@condition LF@tmpCond\n\
JUMPIFEQ $while LF@condition bool@true\n\
JUMP $end\n\
LABEL $return$nil\n\
MOVE LF@%%retval nil@nil\n\
LABEL $end\n\
WRITE LF@%%retval\n\
POPFRAME\n\
RETURN\n");
		break;

		//ord
		case(7):
			fprintf(dest, \
"\
LABEL $ord\n\
PUSHFRAME\n\
DEFVAR LF@%%retval\n\
DEFVAR LF@length\n\
STRLEN LF@length LF@%%0\n\
SUB LF@length LF@length int@1\n\
DEFVAR LF@condition\n\
LT LF@condition LF@%%1 int@0\n\
JUMPIFEQ $ord$return LF@condition bool@true\n\
GT LF@condition LF@%%1 LF@length\n\
JUMPIFEQ $ord$return LF@condition bool@true\n\
STRI2INT LF@%%retval LF@%%0 LF@%%1\n\
JUMP $ord$end\n\
LABEL $ord$return\n\
MOVE LF@%%retval nil@nil\n\
LABEL $ord$end\n\
POPFRAME\n\
RETURN\n");
		break;

		//char
		case(8):
			fprintf(dest, \
"\
LABEL $chr\n\
PUSHFRAME\n\
DEFVAR LF@%%retval\n\
DEFVAR LF@asciiRng\n\
MOVE LF@%%retval string@\n\
LT LF@asciiRng LF@%%0 int@0\n\
JUMPIFEQ $chr$return LF@asciiRng bool@true\n\
GT LF@asciiRng LF@%%0 int@255\n\
JUMPIFEQ $chr$return LF@asciiRng bool@true\n\
INT2CHAR LF@%%retval LF@%%0\n\
LABEL $chr$return\n\
POPFRAME\n\
RETURN\n");
		break;
	}

}

void codeGenBegin(FILE* dest) {
	fprintf(dest, \
"\
.IFJcode18\n\
DEFVAR GF@%%input\n\
DEFVAR GF@%%result\n\
DEFVAR GF@%%operand1\n\
DEFVAR GF@%%operand2\n\
DEFVAR GF@%%operand3\n\
JUMP $$main\n");

	for (int i = 1; i <= 8; i++)
		codeGenBuiltIn(dest, i);
}

void codeGenMainFrameBegin(FILE *dest) {
	fprintf(dest, \
"\
LABEL $$main\n\
CREATEFRAME\n\
PUSHFRAME\n");
}

void codeGenMainFrameEnd(FILE *dest) {
	fprintf(dest, \
"\
POPFRAME\n\
CLEARS\n");
}

void codeGenTypeOfVarValue(FILE *dest, dataTypeEnum type) {
	switch (type) {
		case TYPE_INTEGER:
			fprintf(dest, "int@0");
			break;
		case TYPE_FLOAT:
			fprintf(dest, "float@0.0");
			break;
		case TYPE_STRING:
			fprintf(dest, "string@");
			break;
		default:
			return;
	}
}

void codeGenTypeOfTermValue(FILE *dest, Token token) {
	unsigned char c;
	int i = 0;

	switch (token.Type) {
		case tokenInteger:
			fprintf(dest, "int@%d", token->Data.integer);
			break;
		case tokenFloat:
			fprintf(dest, "float@%f", token.Data.float);
			break;
		case tokenString:
			fprintf(dest, "string@");
			while ((c = (unsigned char) (token.Data.string->value)[i]) != '\0') {
				if (c == '#' || c == '\\' || c <= 32 || !isprint(c)) {
					fprintf(dest, "\\%03d", c);
				} else {
					fprintf(dest, "%c", c);
				}
				i++;
			}
			break;
		case tokenIdentifier:
			fprintf(dest, "LF@%s", token.Data.string->value);
			break;
		default:
			return;
	}
}

void codeGenDeclarationOfVar(FILE *dest, char *var) {
	fprintf(dest, "DEFVAR LF@%s\n", var);
}

void codeGenValueOfVar(FILE *dest, char *var, dataTypeEnum type) {
	fprintf(dest, "MOVE LF@%s ", var);
	codeGenTypeOfVarValue(dest, type);
	fprintf(dest, "\n");
}

void codeGenInput(FILE *dest, char *var, dataTypeEnum type) {
	fprintf(dest, "READ LF@%s ", var);
	switch (type) {
	 	case TYPE_INTEGER:
	 		fprintf(dest, "int");
	 		break;
	 	case TYPE_FLOAT:
	 		fprintf(dest, "float");
	 		break;
	 	case TYPE_STRING:
	 		fprintf(dest, "string");
	 		break;
	 	default:
	 		return;
	}
	fprintf(dest, "\n");
}

void codeGenPrint(FILE *dest, char *var) {
    fprintf(dest, "WRITE LF@%s\n", var);
}

/*----------------------------------------Functions for operations with stack------------------------------------------------------------------*/

void codeGenPush(FILE *dest, Token token) {
	fprintf(dest, "PUSHS ");
	codeGenTypeOfTermValue(dest, token);
	fprintf(dest, "\n");
}

void codeGenStackOperation(FILE *dest, precAnalysisRules rule) {
	switch (rule) {
		case PLUS_RULE:
			fprintf(dest, "ADDS\n");
			break;
		case MINUS_RULE:
			fprintf(dest, "SUBS\n");
			break;
		case MUL_RULE:
			fprintf(dest, "MULS\n");
			break;
		case DIV_RULE:
			fprintf(dest, "DIVS\n");
			break;
		/*case IDIV_RULE:
			fprintf(dest, "POPS GF@%%operand1\n\
				INT2FLOATS\n\
				PUSHS GF@%%operand1\n\
				INT2FLOATS\n\
				DIVS\n\
				FLOAT2INTS\n");
			break;*/
		case EQUAL_RULE:
			fprintf(dest, "EQS\n");
			break;
		case NOT_EQUAL_RULE:
			fprintf(dest, "EQS\n\
				NOTS\n");
			break;
		case LESS_RULE:
			fprintf(dest, "LTS\n");
			break;
		case LESS_OR_EQUAL_RULE:
			fprintf(dest, \
"POPS GF@%%operand1\n\
POPS GF@%%operand2\n\
PUSHS GF@%%operand2\n\
PUSHS GF@%%operand1\n\
LTS\n\
PUSHS GF@%%operand2\n\
PUSHS GF@%%operand1\n\
EQS\n\
ORS\n");
			break;
		case GREATER_RULE:
			fprintf(dest, "GTS\n");
			break;
		case GREATER_OR_EQUAL_RULE:
			fprintf(dest, \
"POPS GF@%%operand1\n\
POPS GF@%%operand2\n\
PUSHS GF@%%operand2\n\
PUSHS GF@%%operand1\n\
GTS\n\
PUSHS GF@%%operand2\n\
PUSHS GF@%%operand1\n\
EQS\n\
ORS\n");
			break;
		default:
			break;
	}
}

void codeGenConcatStackStrings(FILE *dest) {
	fprintf(dest, \
"POPS GF@%%operand3\n\
POPS GF@%%operand2\n\
CONCAT GF@%%operand1 GF@%%operand2 GF@%%operand3\n\
PUSHS GF@%%operand1\n");
}

void codeGenSaveExpressionResult(FILE *dest, char *var, char *frame, dataTypeEnum left, dataTypeEnum right) {
	if (left == TYPE_INTEGER && right == TYPE_FLOAT) {
		fprintf(dest, "FLOAT2INTS\n");
	} else if (left == TYPE_FLOAT && right == TYPE_INTEGER) {
		fprintf(dest, "INT2FLOATS\n");
	}
	fprintf(dest, "POPS %s@%s\n", frame, var);
}

void codeGenOperand1toFloat(FILE *dest) {
	fprintf(dest, "INT2FLOATS\n");
}

void codeGenOperand1toInteger(FILE *dest) {
	fprintf(dest, "FLOAT2INTS\n");
}

void codeGenOperand2toFloat(FILE *dest) {
	fprintf(dest, \
"POPS GF@%%operand1\n\
INT2FLOATS\n\
PUSHS GF@%%operand1\n");
}

void codeGenOperand2toInteger(FILE *dest) {
	fprintf(dest, \
"POPS GF@%%operand1\n\
FLOAT2INTS\n\
PUSHS GF@%%operand1\n");
}

/*---------------------------------------Functions for conditional statements------------------------------------*/

void codeGenLabel(FILE *dest, char *func, int numOfLabel, int deep) {
	fprintf(dest, "LABEL $%s%%%d%%%d\n", func, deep, numOfLabel);
}

void codeGenIfBegin(FILE *dest, char *func, int numOfLabel, int deep) {
	fprintf(dest, "JUMPIFEQ $%s%%%d%%%d GF@%%result bool@false\n", func, deep, numOfLabel);
}

void codeGenIfElse(FILE *dest, char *func, int numOfLabel, int deep) {
	fprintf(dest, "JUMP $%s%%%d%%%d\n", func, deep, numOfLabel + 1);
	codeGenLabel(dest, func, numOfLabel, deep);
}

void codeGenIfEnd(FILE *dest, char *func, int numOfLabel, int deep) {
	codeGenLabel(dest, func, numOfLabel, deep);
}

void codeGenWhileBegin(FILE *dest, char *func, int numOfLabel, int deep) {
	codeGenLabel(dest, func, numOfLabel, deep);
	fprintf(dest, "JUMPIFEQ $%s%%%d%%%d GF@%%result bool@false\n", func, deep, numOfLabel);
}

void codeGenWhileEnd(FILE *dest, char *func, int numOfLabel, int deep) {
	fprintf(dest, "JUMP $%s%%%d%%%d\n", func, deep, numOfLabel - 1);
	codeGenLabel(dest, func, numOfLabel, deep);
}

/*----------------------------------------Functions for generating functions----------------------------------------------*/
//for user functions
void codeGenFuncBegin(FILE *dest, char *func) {
	fprintf(dest, \
"LABEL $%s\n\
PUSHFRAME\n", func);
}

//for user functions
void codeGenFuncEnd(FILE *dest, char *func) {
	fprintf(dest, \
"LABEL $%s%%return\n\
POPFRAME\n\
RETURN\n", func);
}

//for user functions
void codeGenFuncReturnValue(FILE *dest, dataTypeEnum type) {
	fprintf(dest, \
"DEFVAR LF@%%retval\n\
MOVE LF@%%retval ");
	codeGenTypeOfVarValue(dest ,type);
	fprintf(dest, "\n");
}

//for calling function
void codeGenFuncCall(FILE *dest, char *func) {
	fprintf(dest, "CALL $%s\n", func);
}

void codeGenFuncReturnValueAssign(FILE *dest, char *leftValue, dataTypeEnum left, dataTypeEnum ret) {
	if (left == TYPE_FLOAT && ret == TYPE_INTEGER) {
		fprintf(dest, "INT2FLOAT TF@%%retval TF@%%retval\n");
	} else if (left == TYPE_INTEGER && ret == TYPE_FLOAT) {
		fprintf(dest, "FLOAT2INT TF@%%retval TF@%%retval\n");
	}
	fprintf(dest, "MOVE LF@%s TF@%%retval\n", leftValue);
}

//agruments instead of params?
void codeGenFuncDeclarationOfParam(FILE *dest, char *param, int i) {
	fprintf(dest, "DEFVAR LF@%s\n", param);
	fprintf(dest, "MOVE LF@%s LF@%%%d\n", param, i);
}

void codeGenFuncBeforeEnterParam(FILE *dest) {
	fprintf(dest, "CREATEFRAME\n");
}

void codeGenFuncConvertEnterParam(FILE *dest, dataTypeEnum origin, dataTypeEnum converted, int i) {
	if (origin == TYPE_INTEGER && converted == TYPE_FLOAT) {
		fprintf(dest, "INT2FLOAT TF@%%%d TF@%%%d\n", i, i);
	} else if (origin == TYPE_FLOAT && converted == TYPE_INTEGER) {
		fprintf(dest, "FLOAT2INT TF@%%%d TF@%%%d\n", i, i);
	}
}

void codeGenFuncEnterParam(FILE *dest, Token token, int i) {
	fprintf(dest, \
"DEFVAR TF@%%%d\n\
MOVE TF@%%%d ", i, i);
	codeGenTypeOfTermValue(dest, token);
	fprintf(dest, "\n");
}

void codeGenFuncReturn(FILE *dest, char *func) {
 	fprintf(dest, \
"MOVE LF@%%retval GF@%%result\n\
JUMP $%s%%return\n", func);
}

int main(){
	printf("[GENERATOR] Running.\n");
	test = fopen("test", "w");
	codeGenBegin(test);
	codeGenMainFrameBegin(test);
	//codeGenFuncBegin(test, "kokot");
	//codeGenFuncCall(test, "kokot");
	//codeGenFuncReturnValue(test, TYPE_UNDEFINED);
	//codeGenFuncEnd(test, "kokot");
	/*codeGenDeclarationOfVar(test, "jebek");
	codeGenInput(test, "jebek", TYPE_STRING);
	codeGenValueOfVar(test, "jebek", TYPE_STRING);

	codeGenPrint(test, "jebek");*/
	codeGenFuncBeforeEnterParam(test);
	codeGenTypeOfTermValue(test, )
	codeGenFuncCall(test, "substr");

	codeGenMainFrameEnd(test);
	fclose(test);
return 0;
}
