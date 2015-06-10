#include "decisionNode.h"

decisionNode* newDecisionNode(){
	decisionNode* dn = calloc(1, sizeof(decisionNode));
//	dn->i=0; // implicit with calloc (initializes to 0)
	return dn;
}

listDecisionNode* newListDecisionNode(){
	listDecisionNode* listdn=malloc(sizeof(listDecisionNode));
	listdn->ldn=NULL;
	listdn->taille=0;
	listdn->label=NULL;
	listdn->value1=NULL;
	listdn->value2=NULL;
	return listdn;
}

int countNodesDN(decisionNode* dn){
	int sum=1;

	if (dn!=NULL && dn->ht!=NULL){
		int i;
		listDecisionNode* listdn=getChoices(dn->ht);
		for (i=0; i<listdn->taille;i++){
			decisionNode* dnp=listdn->ldn[i];
			sum+=countNodesDN(dnp);
		}
	}
	return sum;
}

int countLeavesDN(decisionNode* dn){
  if (dn!=NULL && dn->ht!=NULL){
    int sum=0;
    int i;
    listDecisionNode* listdn=getChoices(dn->ht);
    for (i=0; i<listdn->taille;i++){
      decisionNode* dnp=listdn->ldn[i];
      sum+=countLeavesDN(dnp);
    }
    return sum;
  }
  else {
    return 1;
  }
}

void freeDecisionNode(decisionNode* dn){
	if (dn->ht!=NULL) freeHashTable(dn->ht);
	free(dn);
}
