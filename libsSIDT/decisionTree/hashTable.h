#ifndef INC_HASHTABLE_H
#define INC_HASHTABLE_H

#include "decisionNode.h"


#include <stdio.h>
#include <stdlib.h>

typedef struct hashElement {
	char label; // label sur ce noeud
	char value; // valeur de bi2 correspondant à cet élément
	char value2; // TODO pour la seconde implementation, a enlever sinon
	struct decisionNode** choice; // si bi2 correspond, sauter à cet élément dans l'arbre de décision
	struct hashElement* nextElement; // sinon la prochaine valeur de bi2 à tester est là
} hashElement;

typedef struct hashTable {
	char nElements;
	hashElement** elements; // indexés sur bi1
} hashTable;

hashElement* newHashElement();
hashTable* newHashTable(char taille);
struct listDecisionNode* getChoices(hashTable* ht);
struct decisionNode** addElement(hashTable* ht, char withLabels, char label, int bi1, int bi2, char learn, struct decisionNode** next);
#endif
