#include "BFSGraph.h"
#include "funcInfo.h"
//#include "graphIO.h"
//#include "ga_types.h"

NGraph* newNGraph(int i, char k){
	NGraph* ng=malloc(sizeof(NGraph));
	ng->NMatrix=calloc(i*k, sizeof(char)); //TODO char ok seulement si i+k < 255
	ng->symbols=calloc(i, sizeof(char));
	return ng;
}

void freeNGraph(NGraph* ng){
	free(ng->NMatrix);
	free(ng->symbols);
	free(ng);
}

void printNGraph(NGraph* ng, int i){
	if (ng==NULL) {
		printf("NGraph NULL\n");
		return;
	}
	else {
		printf("NGraph:\n");
	}

	//printing matrix
	int a;
	int b;
	for (a=0; a<i; a++){
		printf("%d %d ", a+1, ng->symbols[a]);
		for (b=0; b<2; b++){
			printf("%d ", ng->NMatrix[a*2+b]);
		}
		printf("\n");
	}

	//printing symbols
	for (a=0; a<i;a++){
		printf("%d ", ng->symbols[a]);
	}

	printf("\n\n");
	return;
}

void BFSInfo(graph_t* inputGraph, vsize_t vroot, vsize_t R, char valence, ProgInfo* prgInfo, int* tab){
	//WARNING : after calling this function, the ->list_id fields for nodes in unputGraph are broken
	NGraph* ng=newNGraph(R, valence);
	char* NMatrix=ng->NMatrix;
	char* symbols=ng->symbols;

	//all inputgraph nodes to unexplored(0):
	node_list_t* listI = &(inputGraph->nodes);

	vsize_t i;
	node_t* nI;

	vsize_t s = 0;
	nI = node_list_item(listI, vroot);
	if (nI==NULL) printf("NULLPB\n");
	nI->explored=UNDER_EXPLORATION;
	nI->list_id=(vsize_t) 1;
	symbols[0]=nI->symb;

	node_t** queue;
	queue=malloc(R*sizeof(node_t*));
	queue[0]=nI;
	vsize_t nextQueue=1;
	s++;
	int a=0;
	node_t* child;

	static int aa;

	while (s<R+1 && s<=nextQueue){//
		nI=queue[s-1];

		for (a=0; a<nI->children_nb; a++){
			child=nI->children[a];

			if (child->explored==0 && nextQueue<R){
//				tab[0]=(long) tab[0]+1;
//				aa++;
				// adding a child
				int i = findFuncFromInt(prgInfo, child->node_id);
				if (i!=-1){
					tab[i]=(long)tab[i] +1;
//					printf("i: %d\n", tab[i]);
				}
				child->explored=UNDER_EXPLORATION;
				child->list_id=(vsize_t) nextQueue+1;
				queue[nextQueue]=child;
				symbols[nextQueue]=child->symb;
				nextQueue++;
			}

			if (child->explored!=0){
				//adding a link
				if (NMatrix[(s-1)*valence+0]==0){
					NMatrix[(s-1)*valence+0]=child->list_id; // TODO ne dépend pas de k=valence, pas encore(?)
				}
				else {
					NMatrix[(s-1)*valence+1]=child->list_id; // TODO idem
				}
			}
		}

		nI->explored=EXPLORED;
		s++;
	}

	for (i=0; i<nextQueue; i++){
		nI = queue[i];
		nI->explored=UNEXPLORED;
	}

//	if (aa!=23) printf("oops\n");
//	printf("aa %d\n", aa);

	free(queue);
	freeNGraph(ng);
}


NGraph* BFS(graph_t* inputGraph, vsize_t vroot, vsize_t R, char valence){
	//WARNING : after calling this function, the ->list_id fields for nodes in inputGraph are broken
	NGraph* ng=newNGraph(R, valence);
	char* NMatrix=ng->NMatrix;
	char* symbols=ng->symbols;

	//all inputgraph nodes to unexplored(0):
	node_list_t* listI = &(inputGraph->nodes);

	vsize_t i;
	node_t* nI;

	vsize_t s = 0;
	nI = node_list_item(listI, vroot);
	if (nI==NULL) printf("NULLPB\n");
	nI->explored=UNDER_EXPLORATION;
	nI->list_id=(vsize_t) 1;
	symbols[0]=nI->symb;

	node_t** queue;
	queue=malloc(R*sizeof(node_t*));
	queue[0]=nI;
	vsize_t nextQueue=1;
	s++;
	int a=0;
	node_t* child;

	while (s<R+1 && s<=nextQueue){//
		nI=queue[s-1];

		for (a=0; a<nI->children_nb; a++){
			child=nI->children[a];

			if (child->explored==0 && nextQueue<R){
				// adding a child
				child->explored=UNDER_EXPLORATION;
				child->list_id=(vsize_t) nextQueue+1;
				queue[nextQueue]=child;
				symbols[nextQueue]=child->symb;
				nextQueue++;
			}

			if (child->explored!=0){
				//adding a link
        /*if (NMatrix[(s-1)*valence+0]==0){*/
        if (a==0){
					NMatrix[(s-1)*valence+0]=child->list_id; // TODO ne dépend pas de k=valence, pas encore(?)
				}
				else {
					NMatrix[(s-1)*valence+1]=child->list_id; // TODO idem
				}
			}
		}

		nI->explored=EXPLORED;
		s++;
	}

	for (i=0; i<nextQueue; i++){
		nI = queue[i];
		nI->explored=UNEXPLORED;
	}

	free(queue);

	if(nextQueue!=R){
		freeNGraph(ng);
		return NULL;
	}
	else {
		return ng;
	}
}
