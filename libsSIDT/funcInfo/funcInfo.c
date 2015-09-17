#include "funcInfo.h"

ProgInfo* newProgInfo(int n){
	ProgInfo* prgFuncs=malloc(sizeof(ProgInfo));
	prgFuncs->n=n;
	prgFuncs->functions=newProgFunction(n);

	return prgFuncs;
}

ProgFunction** newProgFunction(int n){
	ProgFunction** funcs = malloc(n*sizeof(ProgFunction*));
	int i;
	for (i=0; i<n; i++){
		funcs[i]=malloc(sizeof(ProgFunction));
	}
	return funcs;
}

ProgInfo* progFuncFromFile(char* path){
	FILE *f = fopen(path, "r");
//	char line[128];
	char* line=(char*) malloc(128*sizeof(char));
	int i=0;

	while (fgets(line, 128, f) != NULL){
		i++;
	}

	rewind(f);
	int n=i;
	ProgInfo* prgFuncs = newProgInfo(n);
	i=0;
	while (fgets(line, 128, f) != NULL){
		char* name=strtok(line, " ");
		char* startHex=strtok(NULL, " ");
		char* endHex=strtok(NULL, " ");

		int start, end;
		sscanf(startHex, "%x", &start);
		sscanf(endHex, "%x", &end);

		if (name && start && end){
			prgFuncs->functions[i]->name=malloc((strlen(name)+1)*sizeof(char));
			strcpy(prgFuncs->functions[i]->name, name);
			prgFuncs->functions[i]->start=start;
			prgFuncs->functions[i]->end=end;
		}
		else{
			fprintf(stderr, "Parsing .funcs file : bad splitting.\n");
		}
		i++;
	}

	free(line);
	fclose(f);

	return prgFuncs;
}

int findFuncFromInt(ProgInfo* prgFuncs, int addr){
	int i;
	for (i=0; i<prgFuncs->n; i++){
		if (addr>=prgFuncs->functions[i]->start && addr<prgFuncs->functions[i]->end){
			return i;
		}
	}

	return -1;
}

int findFuncFromHex(ProgInfo* prgFuncs, char* addrHex){
	int addr;
	sscanf(addrHex, "%x", &addr);
	return findFuncFromInt(prgFuncs, addr);
}

void freeProgFunctions(ProgInfo* prgFuncs){
	int i;
	for (i=0; i<prgFuncs->n; i++){
		freeFunction(prgFuncs->functions[i]);
	}
	free(prgFuncs->functions);
	free(prgFuncs);
}

void freeFunction(ProgFunction* func){
	free(func->name);
	free(func);
}
