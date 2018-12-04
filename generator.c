#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "generator.h"
#include "symtable.h"
#include "string.h"
#include "expression.h"
#include "err.h"



#define INS(_code)\
			stringAddString(&outcode, (_code))

#define INSINT(_code)\
			do{	char tmp[50];\
				sprintf(tmp, "%d", _code);\
				INS(tmp);} while(false)

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


dynString outcode;

void codeGenBuiltIn(int builtInFun){
	switch(builtInFun) {

		//inputs
		case(1):
		stringAddString(&outcode,"\
LABEL $inputs\n\
PUSHFRAME\n\
DEFVAR LF@%retval\n\
READ LF@%retval string\n\
POPFRAME\n\
RETURN\n");
		break;

		//inputi
		case(2):
		stringAddString(&outcode, "\
LABEL $inputi\n\
PUSHFRAME\n\
DEFVAR LF@%retval\n\
READ LF@%retval int\n\
POPFRAME\n\
RETURN\n");
		break;

		//inputf
		case(3):
		stringAddString(&outcode, "\
LABEL $inputf\n\
PUSHFRAME\n\
DEFVAR LF@%retval\n\
READ LF@%retval float\n\
POPFRAME\n\
RETURN\n");
		break;

		//print
		case(4):
		stringAddString(&outcode, "\
LABEL $print\n\
PUSHFRAME\n\
DEFVAR LF@%retval\n\
WRITE LF@%retval\n\
POPFRAME\n\
RETURN\n");
		break;

		//length
		case(5):
			stringAddString(&outcode, "\
LABEL $length\n\
PUSHFRAME \n\
DEFVAR LF@%retval\n\
STRLEN LF@%retval LF@%0\n\
POPFRAME\n\
RETURN\n");
		break;

		//substr
		case(6):
		stringAddString(&outcode, "\
LABEL $substr\n\
PUSHFRAME\n\
DEFVAR LF@%retval\n\
MOVE LF@%retval string@\n\
DEFVAR LF@length\n\
STRLEN LF@length LF@%0\n\
DEFVAR LF@condition\n\
LT LF@condition LF@%1 int@0\n\
JUMPIFEQ $return$nil LF@condition bool@true\n\
GT LF@condition LF@%1 LF@length\n\
JUMPIFEQ $return$nil LF@condition bool@true\n\
LT LF@condition LF@%2 int@0\n\
JUMPIFEQ $return$nil LF@condition bool@true\n\
DEFVAR LF@index\n\
MOVE LF@index LF@%1\n\
DEFVAR LF@char\n\
MOVE LF@char string@\n\
DEFVAR LF@substrEnd\n\
ADD LF@substrEnd LF@index LF@%2\n\
DEFVAR LF@tmpCond\n\
LABEL $while\n\
GETCHAR LF@char LF@%0 LF@index\n\
CONCAT LF@%retval LF@%retval LF@char\n\
ADD LF@index LF@index int@1\n\
LT LF@condition LF@index LF@substrEnd\n\
LT LF@tmpCond LF@index LF@length\n\
AND LF@condition LF@condition LF@tmpCond\n\
JUMPIFEQ $while LF@condition bool@true\n\
JUMP $end\n\
LABEL $return$nil\n\
MOVE LF@%retval nil@nil\n\
LABEL $end\n\
WRITE LF@%retval\n\
POPFRAME\n\
RETURN\n");
		break;

		//ord
		case(7):
			stringAddString(&outcode, "\
LABEL $ord\n\
PUSHFRAME\n\
DEFVAR LF@%retval\n\
DEFVAR LF@length\n\
STRLEN LF@length LF@%0\n\
SUB LF@length LF@length int@1\n\
DEFVAR LF@condition\n\
LT LF@condition LF@%1 int@0\n\
JUMPIFEQ $ord$return LF@condition bool@true\n\
GT LF@condition LF@%1 LF@length\n\
JUMPIFEQ $ord$return LF@condition bool@true\n\
STRI2INT LF@%retval LF@%0 LF@%1\n\
JUMP $ord$end\n\
LABEL $ord$return\n\
MOVE LF@%retval nil@nil\n\
LABEL $ord$end\n\
POPFRAME\n\
RETURN\n");
		break;

		//char
		case(8):
			stringAddString(&outcode, "\
LABEL $chr\n\
PUSHFRAME\n\
DEFVAR LF@%retval\n\
DEFVAR LF@asciiRng\n\
MOVE LF@%retval string@\n\
LT LF@asciiRng LF@%0 int@0\n\
JUMPIFEQ $chr$return LF@asciiRng bool@true\n\
GT LF@asciiRng LF@%0 int@255\n\
JUMPIFEQ $chr$return LF@asciiRng bool@true\n\
INT2CHAR LF@%retval LF@%0\n\
LABEL $chr$return\n\
POPFRAME\n\
RETURN\n");
		break;
	}

}

void codeGenBegin() {
	stringAddString(&outcode, "\
.IFJcode18\n\
DEFVAR GF@%input\n\
DEFVAR GF@%result\n\
DEFVAR GF@%operand1\n\
DEFVAR GF@%operand2\n\
DEFVAR GF@%operand3\n\
DEFVAR GF@%temp\n\
JUMP $$main\n");

	for (int i = 1; i <= 8; i++)
		codeGenBuiltIn(i);
}

void codeGenMainFrameBegin() {
	stringAddString(&outcode, "LABEL $$main\n\
CREATEFRAME\n\
PUSHFRAME\n");

}

void codeGenMainFrameEnd() {
	stringAddString(&outcode, "LABEL $error\nEXIT int@9\nPOPFRAME\n\
CLEARS\n");
}

void codeGenTypeOfVarValue(dataTypeEnum type) {
	switch (type) {
		case TYPE_INTEGER:
			stringAddString(&outcode, "int@0");
			break;
		case TYPE_FLOAT:
			stringAddString(&outcode, "float@0.0");
			break;
		case TYPE_STRING:
			stringAddString(&outcode, "string@");
			break;
		default:
			return;
	}
}

void codeGenTypeOfTermValue(parseData parserData) {
	unsigned char c;
	char val[50]; //TODO
	int i = 0;
	dynString tmp;
	stringInit(&tmp);
	//TODO stringDispose kde?!
	switch (parserData.token.Type) {
		case tokenInteger:
			sprintf(val, "%d", parserData.token.Data.integer);
			INS("int@");INS(val);INS("\n");
			break;
		case tokenFloat:
			sprintf(val, "%a", parserData.token.Data.flt);
			INS("float@");INS(val);INS("\n");
			break;
		case tokenString:
			stringAddString(&outcode, "string@");
			while ((c = (unsigned char) (parserData.token.Data.string->value)[i]) != '\0') {
				if (c == '#' || c == '\\' || c <= 32 || !isprint(c)) {
					stringAddChar(&tmp, '\\'); sprintf(val, "%03d", c); stringAddString(&tmp, val);
				} else {
					stringAddChar(&tmp, c);
				}
				i++;
			}
			INS(tmp.value);
			break;
		case tokenIdentifier:
			INS("LF@");INS(parserData.token.Data.string->value);
			break;
		default:
			stringDispose(&tmp);
			return;
	}
	stringDispose(&tmp);
}

void codeGenDeclarationOfVar(char *var) {
	//stringAddString(&outcode, "DEFVAR LF@%s\n", var);
	INS("DEFVAR LF@");INS(var);INS("\n");
}

void codeGenValueOfVar(char *var, dataTypeEnum type) {
	//stringAddString(&outcode, "MOVE LF@%s ", var);
	INS("MOVE LF@");INS(var);
	codeGenTypeOfVarValue(type);
	stringAddString(&outcode, "\n");
}

void codeGenInput(char *var, dataTypeEnum type) {
	//stringAddString(&outcode, "READ LF@%s ", var);
	INS("READ LF@"); INS(var);
	switch (type) {
	 	case TYPE_INTEGER:
	 		stringAddString(&outcode, "int");
	 		break;
	 	case TYPE_FLOAT:
	 		stringAddString(&outcode, "float");
	 		break;
	 	case TYPE_STRING:
	 		stringAddString(&outcode, "string");
	 		break;
	 	default:
	 		return;
	}
	stringAddString(&outcode, "\n");
}

void codeGenPrint() {
    //stringAddString(&outcode, "WRITE LF@%s\n", var);
    INS("WRITE GF@%result"); INS("\n");
}

void codeGenPrintBracket() {
    //stringAddString(&outcode, "WRITE LF@%s\n", var);
    INS("POPS GF@%result"); INS("\n");
    INS("WRITE GF@%result"); INS("\n");
}

/*----------------------------------------Functions for operations with stack------------------------------------------------------------------*/

void codeGenPush(parseData parserData) {
	stringAddString(&outcode, "PUSHS ");
	codeGenTypeOfTermValue(parserData);
	stringAddString(&outcode, "\n");
}

void codeGenStackOperation(precAnalysisRules rule) {
	switch (rule) {
		case PLUS_RULE:
			stringAddString(&outcode, "ADDS\n");
			break;
		case MINUS_RULE:
			stringAddString(&outcode, "SUBS\n");
			break;
		case MUL_RULE:
			stringAddString(&outcode, "MULS\n");
			break;
		case DIV_RULE:
			stringAddString(&outcode, "POPS GF@%operand1\n");
			stringAddString(&outcode, "MOVE GF@%temp GF@%operand1\n");
			stringAddString(&outcode, "PUSHS GF@%operand1\n");
			stringAddString(&outcode, "PUSHS float@0x0p+0\n");
			stringAddString(&outcode, "JUMPIFEQS $error\n");
			stringAddString(&outcode, "PUSHS GF@%temp\n");
			stringAddString(&outcode, "DIVS\n");
			break;
		case IDIV_RULE:
			stringAddString(&outcode, "POPS GF@%operand1\n");
			stringAddString(&outcode, "MOVE GF@%temp GF@%operand1\n");
			stringAddString(&outcode, "PUSHS GF@%operand1\n");
			stringAddString(&outcode, "PUSHS int@0\n");
			stringAddString(&outcode, "JUMPIFEQS $error\n");
			stringAddString(&outcode, "PUSHS GF@%temp\n");
			stringAddString(&outcode, "IDIVS\n");
			break;
		case EQUAL_RULE:
			stringAddString(&outcode, "EQS\n");
			break;
		case NOT_EQUAL_RULE:
			stringAddString(&outcode, "EQS\n\
				NOTS\n");
			break;
		case LESS_RULE:
			stringAddString(&outcode, "LTS\n");
			break;
		case LESS_OR_EQUAL_RULE:
			stringAddString(&outcode, "POPS GF@%operand1\n\
POPS GF@%operand2\n\
PUSHS GF@%operand2\n\
PUSHS GF@%operand1\n\
LTS\n\
PUSHS GF@%operand2\n\
PUSHS GF@%operand1\n\
EQS\n\
ORS\n");
			break;
		case GREATER_RULE:
			stringAddString(&outcode, "GTS\n");
			break;
		case GREATER_OR_EQUAL_RULE:
			stringAddString(&outcode, "POPS GF@%operand1\n\
POPS GF@%operand2\n\
PUSHS GF@%operand2\n\
PUSHS GF@%operand1\n\
GTS\n\
PUSHS GF@%operand2\n\
PUSHS GF@%operand1\n\
EQS\n\
ORS\n");
			break;
		default:
			break;
	}
}

void codeGenConcatStackStrings() {
	stringAddString(&outcode, "POPS GF@%operand3\n\
POPS GF@%operand2\n\
CONCAT GF@%operand1 GF@%operand2 GF@%operand3\n\
PUSHS GF@%operand1\n");
}

void codeGenSaveExpressionResult(char *var, char *frame, dataTypeEnum left, dataTypeEnum right) {
	if (left == TYPE_INTEGER && right == TYPE_FLOAT) {
		stringAddString(&outcode, "FLOAT2INTS\n");
	} else if (left == TYPE_FLOAT && right == TYPE_INTEGER) {
		stringAddString(&outcode, "INT2FLOATS\n");
	}
	//stringAddString(&outcode, "POPS %s@%s\n", frame, var);
	INS("POPS "); INS(frame); INS("@"); INS(var); INS("\n");
}

void codeGenOperand1toFloat() {
	stringAddString(&outcode, "INT2FLOATS\n");
}
/*
void codeGenOperand1toInteger() {
	stringAddString(&outcode, "FLOAT2INTS\n");
}
*/
void codeGenOperand3toFloat() {
	stringAddString(&outcode, "INT2FLOATS\n\
POPS GF@%operand1\n\
PUSHS GF@%operand1\n");
}
/*
void codeGenOperand3toInteger() {
	stringAddString(&outcode, "POPS GF@%operand1\n\
FLOAT2INTS\n\
PUSHS GF@%operand1\n");
}
*/

/*---------------------------------------Functions for conditional statements------------------------------------*/

void codeGenLabel(char *func, int numOfLabel, int deep) {
	//stringAddString(&outcode, "LABEL $%s%%d%%d\n", func, deep, numOfLabel);
	INS("LABEL $"); INS(func); INS("%"); INSINT(deep); INS("%"); INSINT(numOfLabel); INS("\n");
}

void codeGenIfBegin(char *func, int numOfLabel, int deep) {
	//stringAddString(&outcode, "JUMPIFEQ $%s%%d%%d GF@%result bool@false\n", func, deep, numOfLabel);
	INS("JUMPIFEQ $"); INS(func); INS("%"); INSINT(deep); INS("%"); INSINT(numOfLabel); INS(" GF@%result bool@false\n");
}

void codeGenIfElse(char *func, int numOfLabel, int deep) {
	//stringAddString(&outcode, "JUMP $%s%%d%%d\n", func, deep, numOfLabel + 1);
	INS("JUMP $"); INS(func); INS("%"); INSINT(deep); INS("%"); INSINT(numOfLabel+1);INS("\n");
	codeGenLabel(func, numOfLabel, deep);
}

void codeGenIfEnd(char *func, int numOfLabel, int deep) {
	codeGenLabel(func, numOfLabel, deep);
}

void codeGenWhileBegin(char *func, int numOfLabel, int deep) {
	//codeGenLabel(func, numOfLabel, deep);
	//stringAddString(&outcode, "JUMPIFEQ $%s%%d%%d GF@%result bool@false\n", func, deep, numOfLabel);
	INS("JUMPIFEQ $"); INS(func); INS("%"); INSINT(deep); INS("%"); INSINT(numOfLabel); INS(" GF@%result bool@false\n");
}

void codeGenWhileEnd(char *func, int numOfLabel, int deep) {
	//stringAddString(&outcode, "JUMP $%s%%d%%d\n", func, deep, numOfLabel - 1);
	INS("JUMP $"); INS(func); INS("%"); INSINT(deep); INS("%"); INSINT(numOfLabel-1); INS("\n");
	codeGenLabel(func, numOfLabel, deep);
}

/*----------------------------------------Functions for generating functions----------------------------------------------*/
//for user functions
void codeGenFuncBegin(char *func) {
	//stringAddString(&outcode, "LABEL $%s\n\PUSHFRAME\n", func);
	INS("JUMP $"); INS(func); INS("$end\n");
	INS("LABEL $"); INS(func); INS("\n"); INS("PUSHFRAME\n");
}

//for user functions
void codeGenFuncEnd(char *func) {
	//stringAddString(&outcode, "LABEL $%s%return\n\POPFRAME\n\RETURN\n", func);
	INS("LABEL $"); INS(func); INS("%return\n");
	INS("POPFRAME\n");
	INS("RETURN\n");
	INS("LABEL $"); INS(func); INS("$end\n");
}

//for user functions
void codeGenFuncReturnValue(dataTypeEnum type) {
	stringAddString(&outcode, "DEFVAR LF@%retval\n\
MOVE LF@%retval ");
	codeGenTypeOfVarValue(type);
	stringAddString(&outcode, "\n");
}

//for calling function
void codeGenFuncCall(char *func) {
	//stringAddString(&outcode, "CALL $%s\n", func);
	INS("CALL $"); INS(func); INS("\n");
}

void codeGenFuncReturnValueAssign(char *leftValue) {
	/*
	if (left == TYPE_FLOAT && ret == TYPE_INTEGER) {
		stringAddString(&outcode, "INT2FLOAT TF@%retval TF@%retval\n");
	} else if (left == TYPE_INTEGER && ret == TYPE_FLOAT) {
		stringAddString(&outcode, "FLOAT2INT TF@%retval TF@%retval\n");
	}
	*/
	//stringAddString(&outcode, "MOVE LF@%s TF@%retval\n", leftValue);
	INS("MOVE LF@"); INS(leftValue); INS(" TF@%retval\n");
}

//agruments instead of params?
void codeGenFuncDeclarationOfParam(char *param, int i) {
	//stringAddString(&outcode, "DEFVAR LF@%s\n", param);
	//stringAddString(&outcode, "MOVE LF@%s LF@%%d\n", param, i);
	INS("DEFVAR LF@"); INS(param); INS("\n");
	INS("MOVE LF@"); INS(param); INS(" LF@%"); INSINT(i); INS("\n");
}

void codeGenFuncBeforeEnterParam() {
	stringAddString(&outcode, "CREATEFRAME\n");
}

void codeGenFuncConvertEnterParam(dataTypeEnum origin, dataTypeEnum converted, int i) {
	if (origin == TYPE_INTEGER && converted == TYPE_FLOAT) {
		//stringAddString(&outcode, "INT2FLOAT TF@%%d TF@%%d\n", i, i);
		INS("INT2FLOAT TF@%"); INSINT(i); INS("TF@%"); INSINT(i); INS("\n");
	} else if (origin == TYPE_FLOAT && converted == TYPE_INTEGER) {
		//stringAddString(&outcode, "FLOAT2INT TF@%%d TF@%%d\n", i, i);
		INS("FLOAT2INT TF@%"); INSINT(i); INS("TF@%"); INSINT(i); INS("\n");
	}
}

void codeGenFuncEnterParam(parseData parserData, int i) {
	//stringAddString(&outcode, "DEFVAR TF@%%d\n\MOVE TF@%%d ", i, i);
	INS("DEFVAR TF@%"); INSINT(i); INS("\nMOVE TF@%"); INSINT(i); INS(" ");
	codeGenTypeOfTermValue(parserData);
	stringAddString(&outcode, "\n");
}

void codeGenFuncReturn(char *func) {
 	//stringAddString(&outcode, "MOVE LF@%retval GF@%result\n\JUMP $%s%return\n", func);
 	INS("MOVE LF@%retval GF@%result\n");
 	INS("JUMP $"); INS(func); INS("%return\n");
}

int codeGenStart(){
	stringInit(&outcode);
	codeGenBegin();
	return true;
}

int codeGenClear(){
	stringDispose(&outcode);
	return true;
}

void codeGenPutToFile(FILE *out){
	fputs(outcode.value, out);
	codeGenClear();
}