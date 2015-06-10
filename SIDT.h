#ifndef SIDT_H
#define SIDT_H

#include <stdio.h>

extern "C" {
#include "graphIO.h"
#include "decisionTree.h"
#include "hashTable.h"
#include "BFSGraph.h"
#include "prog.h"
#include "funcInfo.h"
}
#include <unordered_set>

char* drawDN(decisionNode* dn, int prof, char);
char* drawDT(decisionTree* dt, char);
int learnGraph(graph_t* gr, decisionTree* dt, char* message, int maxLearn, char withLabels);
int findGraph(graph_t* gr, decisionTree* dt, int* countByProg, ProgInfo* scanInfo, int* scanFuncs, char withLabels);
void printInfoDT(decisionTree* dt);

#endif