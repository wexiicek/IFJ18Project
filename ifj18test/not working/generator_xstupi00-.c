#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "generator.h"
#include "scanner.h"

codeList *code;

var *varInit(){
	var *variable = (var*) malloc(sizeof(var));
	if(var){
		variable -> type = 0;
		variable -> isConstant = false;
		return variable;
	}
	return 99;
}

int codeGenListInit(){
	code = (codeList*) malloc(sizeof(codeList));
	if(code){
		code -> First = NULL;
		code -> Op = NULL;
		code -> Last = NULL;
		return 0;
	}
	return 99;
}

var *codeGenCreateVar(char *str1, bool isConstant){
	var *helper = varInit();
	helper -> data.str = (char*)malloc(sizeof(char)*strlen(str1)+2);
	if(helper -> data.str){
		strcpy(helper -> data.str, str1);
		strcat(helper -> data.str, " ");
		helper -> isConstant = isConstant;
		return helper;
	}
	return 99;
}
/* REQUIRED FUNCTIONS */

bool codeGenStartProgram() {
	insertCode("LABEL $$main", NULL, NULL, NULL);
	insertCode("CREATEFRAME", NULL, NULL, NULL);
	insertCode("PUSHFRAME", NULL, NULL, NULL);
	return TRUE;
}

/* BUILT IN FUNCTIONS */

void length(){
	var label = codeGenCreateVar("$length", false);
	var returnValue = codeGenCreateVar("LF@%retval")
	insertCode("LABEL ", label, NULL, NULL);
	insertCode("PUSHFRAME ", , , );
	insertCode("DEFVAR ", , , );

}

void substr(){}

void ord(){}

void chr(){
	insertCode("LABEL ", "$chr", NULL, NULL);
	insertCode("PUSHFRAME ", NULL, NULL, NULL);
	insertCode("DEFVAR ", LF@%returnValue, NULL, NULL);
	insertCode("DEFVAR ", LF@%asciiRange, NULL, NULL);
	insertCode("MOVE ", LF@returnValue, string@, NULL);
	insertCode("LT ", LF&asciiRange, LF@%0, int@0);
	insertCode("JUMPIFEQ ", $chr$return, LF@asciiRange, bool@true);
	insertCode("GT ", LF@asciiRange, LF@%0, int@255);
	insertCode("JUMPIFEQ ", $chr$return, LF@asciiRange, bool@true);
	insertCode("INT2CHAR ", LF@%returnValue, LF@%0, NULL);
	insertCode("LABEL ", $chr$return, NULL, NULL);
	insertCode("POPFRAME", NULL, NULL, NULL);
	insertCode("RETURN", NULL, NULL, NULL);
}