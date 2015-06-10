#ifndef INC_DECISIONNODE_H
#define INC_DECISIONNODE_H

#include "hashTable.h"

typedef struct decisionNode {
	struct hashTable* ht;
	int i;
} decisionNode;

typedef struct listDecisionNode{
	struct decisionNode** ldn;
	int taille;
	int* label;
	int* value1;
	int* value2;
} listDecisionNode;

decisionNode* newDecisionNode();
listDecisionNode* newListDecisionNode();
int countNodesDN(decisionNode* dn);
int countLeavesDN(decisionNode* dn);
#endif
