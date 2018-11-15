typedef struct varStruct {
	int type;
	union {
		int i;
		double d;
		char *str;
	} varData;
	bool isConstant;
} var;

typedef struct instrStruct {
	char *instrName;
	struct varStruct *var1;
	struct varStruct *var2;
	struct varStruct *var3;
	struct instrStruct *next;
} instr;

typedef struct codeStruct {
	instrStruct *First;
	instrStruct *Op;
	instrStruct *Last;
} codeList;


