#ifndef INC_BFSGRAPH_H
#define INC_BFSGRAPH_H

#include "graphIO.h"
#include "funcInfo.h"

typedef struct NGraph {
	char* NMatrix;
	char* symbols;
} NGraph;

NGraph* newNGraph(int i, char k);
void freeNGraph(NGraph* ng);
void printNGraph(NGraph* ng, int i);
NGraph* BFS(graph_t* inputGraph, vsize_t vroot, vsize_t R, char valence);
void BFSInfo(graph_t* inputGraph, vsize_t vroot, vsize_t R, char valence, ProgInfo* prgInfo, int* tab);

#endif
