#ifndef INC_FUNCINFO_H
#define INC_FUNCINFO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct ProgInfo {
	int n;
	struct ProgFunction** functions;
} ProgInfo;

typedef struct ProgFunction {
	int start;
	int end;
	char* name;
} ProgFunction;

ProgInfo* newProgInfo(int n);
ProgFunction** newProgFunction(int n);
ProgInfo* progFuncFromFile(char* path);
int findFuncFromInt(ProgInfo* prgFuncs, int addr);
int findFuncFromHex(ProgInfo* prgFuncs, char* addr);
void freeProgFunctions(ProgInfo* prgFuncs);
void freeFunction(ProgFunction* func);

#endif
