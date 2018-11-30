#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include <stdbool.h>
#include "expression.h"
#include "symtable.h"
#include "scanner.h"

void codeGenBuiltIn(int builtInFun);
void codeGenBegin();
void codeGenMainFrameBegin();
void codeGenMainFrameEnd();
void codeGenTypeOfVarValue(dataTypeEnum type);
void codeGenTypeOfTermValue(parseData parserData);
void codeGenDeclarationOfVar(char *var);
void codeGenValueOfVar(char *var, dataTypeEnum type);
void codeGenInput(char *var, dataTypeEnum type);
void codeGenPrint(char *var);
/*-------------------------------------------Functions for operations with stack---------------------------------------------------*/
void codeGenPush(parseData parserData);
void codeGenStackOperation(precAnalysisRules rule);
void codeGenConcatStackStrings();
void codeGenSaveExpressionResult(char *var, char *frame, dataTypeEnum left, dataTypeEnum right);
void codeGenOperand1toFloat();
void codeGenOperand1toInteger();
void codeGenOperand2toFloat();
void codeGenOperand2toInteger();
/*------------------------------------------Functions for conditional statements------------------------------------------------------*/
void codeGenLabel(char *func, int numOfLabel, int deep);
void codeGenIfBegin(char *func, int numOfLabel, int deep);
void codeGenIfElse(char *func, int numOfLabel, int deep);
void codeGenIfEnd(char *func, int numOfLabel, int deep);
void codeGenWhileBegin(char *func, int numOfLabel, int deep);
void codeGenWhileEnd(char *func, int numOfLabel, int deep);
/*-----------------------------------------Functions for generating functions---------------------------------------------------------*/
void codeGenFuncBegin( char *func);
void codeGenFuncEnd( char *func);
void codeGenFuncReturnValue( dataTypeEnum type);
void codeGenFuncCall(char *func);
void codeGenFuncReturnValueAssign(char *leftValue, dataTypeEnum left, dataTypeEnum ret);
void codeGenFuncDeclarationOfParam(char *param, int i);
void codeGenFuncBeforeEnterParam();
void codeGenFuncConvertEnterParam(dataTypeEnum origin, dataTypeEnum converted, int i);
void codeGenFuncEnterParam(parseData parserData, int i);
void codeGenFuncReturn(char *func);

int codeGenStart();
int codeGenClear();
void codeGenPutToFile(FILE *out);

#endif
