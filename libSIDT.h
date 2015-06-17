#ifndef LIBSIDT_H
#define LIBSIDT_H

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

extern char optionFuncs;

char* drawDN(decisionNode* dn, int prof, char);
char* drawDT(decisionTree* dt, char);
int learnGraph(graph_t* gr, decisionTree* dt, char* message, int maxLearn, char withLabels);
int findGraph(graph_t* gr, decisionTree* dt, int* countByProg, ProgInfo* scanInfo, int* scanFuncs, char withLabels);
char* labCharToString(char label);
void printInfoDT(decisionTree* dt);
char* makeDotNode(void* n, int prof);
char* makeDotEdge(void* pere, void* fils, int c1, int c2, int label, char withLabels);

#endif