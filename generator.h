#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include <stdbool.h>
#include "expression.h"
#include "symtable.h"
#include "scanner.h"

void codeGenBuiltIn(FILE *dest, int builtInFun);
void codeGenBegin(FILE* dest, int builtInFun);
void codeGenMainFrameBegin(FILE *dest);
void codeGenMainFrameEnd(FILE *dest);
void codeGenTypeOfVarValue(FILE *dest, dataTypeEnum type);
void codeGenTypeOfTermValue(FILE *dest, Token token);
void codeGenDeclarationOfVar(FILE *dest, char *var);
void codeGenValueOfVar(FILE *dest, char *var, dataTypeEnum type);
void codeGenInput(FILE *dest, char *var, dataTypeEnum type);
void codeGenPrint(FILE *dest);
/*-------------------------------------------Functions for operations with stack---------------------------------------------------*/
void codeGenPush(FILE *dest, Token token);
void codeGenStackOperation(FILE *dest, precAnalysisRules rule);
void codeGenConcatStackStrings(FILE *dest);
void codeGenSaveExpressionResult(FILE *dest, char *var, char *frame, dataTypeEnum left, dataTypeEnum right);
void codeGenOperand1toFloat(FILE *dest);
void codeGenOperand1toInteger(FILE *dest);
void codeGenOperand2toFloat(FILE *dest);
void codeGenOperand2toInteger(FILE *dest);
/*------------------------------------------Functions for conditional statements------------------------------------------------------*/
void codeGenLabel(FILE *dest, char *func, int numOfLabel, int deep);
void codeGenIfBegin(FILE *dest, char *func, int numOfLabel, int deep);
void codeGenIfElse(FILE *dest, char *func, int numOfLabel, int deep);
void codeGenIfEnd(FILE *dest, char *func, int numOfLabel, int deep);
void codeGenWhileBegin(FILE *dest, char *func, int numOfLabel, int deep);
void codeGenWhileEnd(FILE *dest, char *func, int numOfLabel, int deep);
/*-----------------------------------------Functions for generating functions---------------------------------------------------------*/
void codeGenFuncBegin(FILE *dest, char *func);
void codeGenFuncEnd(FILE *dest, char *func);
void codeGenFuncReturnValue(FILE *dest, dataTypeEnum type);
void codeGenFuncCall(FILE *dest, char *func);
void codeGenFuncReturnValueAssign(FILE *dest, char *leftValue, dataTypeEnum left, dataTypeEnum ret);
void codeGenFuncDeclarationOfParam(FILE *dest, char *param, int i);
void codeGenFuncBeforeEnterParam(FILE *dest);
void codeGenFuncConvertEnterParam(FILE *dest, dataTypeEnum origin, dataTypeEnum converted, int i);
void codeGenFuncEnterParam(FILE *dest, Token token, int i);
void codeGenFuncReturn(FILE *dest, char *func);

#endif