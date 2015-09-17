#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "decisionNode.h"
#include "BFSGraph.h"
//#include "matchInfo.h"
#include "prog.h"

typedef struct decisionTree {
	char siteValence; // max number of children for each node
	int siteSize; // size of the managed sites
	int nSites; // number of sites in the tree (duplicate are only present once)
	int nLearntSites; // number of sites learnt
	site** sites; // learnt sites
	int nProgs; // number of learnt programs
	prog** progs; // learnt programs
	decisionNode* root; // root of the decision tree
} decisionTree;

decisionTree* newDecisionTree(int siteSize, char siteValence);
void addSiteToDT(decisionTree* dt, site* sit);
int learnSite(decisionTree* dt, NGraph* site, prog* prg, char withLabels);
int findSite(decisionTree* dt, NGraph* site, char withLabels);
int learnSiteR(decisionTree* dt, decisionNode** dn, NGraph* site, int i, prog* prg, char learn, char withLabels);
int countNodesDT(decisionTree* dt);
int countLeavesDT(decisionTree* dt);
void freeDecisionTree(decisionTree* dt);
void addProgToDT(decisionTree* dt, prog* prg);

#endif