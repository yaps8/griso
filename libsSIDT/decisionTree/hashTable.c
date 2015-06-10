#include "hashTable.h"

hashElement* newHashElement(){
	hashElement* he = malloc(sizeof(hashElement));
	he->nextElement=NULL;
	return he;
}

//premiere implementation
hashTable* newHashTablev1(char taille){
	hashTable* ht = malloc(sizeof(hashTable));
	ht->elements=malloc(taille*sizeof(hashElement*));
	ht->nElements=taille;

	char a=0;
	for (a=0; a<taille; a++){
		ht->elements[a]=NULL;
	}

	return ht;
}

//seconde implementation
hashTable* newHashTable(char taille){
	hashTable* ht = malloc(sizeof(hashTable));
	ht->elements=malloc(sizeof(hashElement*));
	ht->nElements=taille;
	ht->elements[0]=NULL;

	return ht;
}

// premiere implementation
// returns the added decisionNode or the existing matching one
struct decisionNode** addElementv1(hashTable* ht, int bi1, int bi2, decisionNode** next){
	hashElement** pelement = &ht->elements[bi1];

	while(1==1){
		if (*pelement==NULL){
			// ajout de l'élément si on ne l'a pas trouvé
			hashElement* newElement=newHashElement();
			newElement->value=bi2;
			newElement->choice=next;
			newElement->nextElement=NULL;
			*pelement=newElement;
			return (*pelement)->choice;
		}
		else{
			if ((*pelement)->value==bi2){
				// élément déjà présent
				return (*pelement)->choice;
			}
			else{
				// on cherche sur l'élément suivant
				pelement=&(*pelement)->nextElement;
			}
		}
	}
}

// seconde implementation, plus bête et plus efficace...
// returns the added decisionNode or the existing matching one
struct decisionNode** addElement(hashTable* ht, char withLabels, char label, int bi1, int bi2, char learn, decisionNode** next){
	hashElement** pelement = &ht->elements[0];

	while(1==1){
		if (*pelement==NULL){
			if (learn){
				// ajout de l'élément si on ne l'a pas trouvé
				hashElement* newElement=newHashElement();
				if (withLabels) newElement->label=label;
				else newElement->label=-1;
				newElement->value=bi1;
				newElement->value2=bi2;
				newElement->choice=next;
				newElement->nextElement=NULL;
				*pelement=newElement;
				return (*pelement)->choice;
			}
			else {
				return NULL;
			}
		}
		else{
			if (((!withLabels) || (*pelement)->label==label) && (*pelement)->value==bi1 && (*pelement)->value2==bi2){
				// élément déjà présent
				return (*pelement)->choice;
			}
			//debug
//			else if ((*pelement)->value==bi1 && (*pelement)->value2==bi2){
//				printf("labelnotmatched: %d != %d\n", (*pelement)->label, label);
//				pelement=&(*pelement)->nextElement;
//			}
			//fin debug
			else{
				// on cherche sur l'élément suivant
				pelement=&(*pelement)->nextElement;
			}
		}
	}
}

struct listDecisionNode* getChoices(hashTable* ht){
	listDecisionNode* dnlist=newListDecisionNode();
	hashElement** pelement = &ht->elements[0];

	while(1==1){
		if (*pelement==NULL){
			return dnlist;
		}
		else{
			dnlist->ldn=realloc(dnlist->ldn, (dnlist->taille+1)*sizeof(decisionNode*));
			dnlist->ldn[dnlist->taille]=*(*pelement)->choice;
			dnlist->value1=realloc(dnlist->value1, (dnlist->taille+1)*sizeof(int));
			dnlist->value1[dnlist->taille]=(*pelement)->value;
			dnlist->value2=realloc(dnlist->value2, (dnlist->taille+1)*sizeof(int));
			dnlist->value2[dnlist->taille]=(*pelement)->value2;
			dnlist->label=realloc(dnlist->label, (dnlist->taille+1)*sizeof(int));
			dnlist->label[dnlist->taille]=(*pelement)->label;
			dnlist->taille++;
			pelement=&(*pelement)->nextElement;
		}
	}

	return dnlist;
}

void freeHashElement(hashElement* he){
	if (*(he->choice)!=NULL) freeDecisionNode(*(he->choice));
	if (he->choice!=NULL) free(he->choice);
	if (he->nextElement!=NULL) freeHashElement(he->nextElement);
	free(he);
}

// premiere implementation
void freeHashTablev1(hashTable* ht){
	char a;
	for (a=0; a<ht->nElements; a++){
		if (ht->elements[a]!=NULL) freeHashElement(ht->elements[a]);
	}
	free(ht->elements);
	free(ht);
}

// seconde implementation
void freeHashTable(hashTable* ht){
	if (ht->elements!=NULL) freeHashElement(ht->elements[0]);
	free(ht->elements);
	free(ht);
}
