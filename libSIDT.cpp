#include "libSIDT.h"

int learnGraph(graph_t* gr, decisionTree* dt, char* message, int maxLearn, char withLabels){
	NGraph* ng;
	int n=0;
	int a;
	prog* prg=newProg(message, dt->nProgs);
	addProgToDT(dt, prg);

	for (a=0; a<gr->nodes.count; a++){
		ng=BFS(gr, a, dt->siteSize, dt->siteValence);
		if (ng!=NULL && (maxLearn == 0 || dt->nLearntSites<maxLearn)){
//			printNGraph(ng, dt->siteSize);
			if (learnSite(dt, ng, prg, withLabels) == 0){
        n++;
        dt->nLearntSites++;
       //printf("new site:\n");
       //printf("(+%d):\n", a);
       //printNGraph(ng, dt->siteSize);
      }
      else {
        //if (a==154){
        //printf("(=%d):\n", a);
        //printNGraph(ng, dt->siteSize);
        //}
      }
			freeNGraph(ng);
		}
	}
	prg->nSites=n;
//	dt->nLearntSites+=n;
	return n;
}

int findGraph(graph_t* gr, decisionTree* dt, int* countByProg, ProgInfo* scanInfo, int* scanFuncs, char withLabels){
	NGraph* ng;
	int n=0;
	char* message= (char*) "";
	int a;
	int k;
	int r;
	site* sit;
  std::unordered_set<int> sites;
  
//	int nSites=0; //DEBUG

	for (a=0; a<gr->nodes.count; a++){
		ng=BFS(gr, a, dt->siteSize, dt->siteValence);
		if (ng!=NULL){
//			printNGraph(ng, dt->siteSize);
//			nSites++; //DEBUG
			r=findSite(dt, ng, withLabels);

			if (r!=0){
// 				n++;
        bool b = std::get<1>(sites.insert(r));

        if (b){
          if (optionFuncs) BFSInfo(gr, a, dt->siteSize, dt->siteValence, scanInfo, scanFuncs);
//           sit=dt->sites[r-1];
//           for (k=0; k<sit->nProgs; k++){
//             countByProg[sit->progs[k]->index]++;
//           }
        }
			}

			freeNGraph(ng);
		}
	}
//	if (nSites!=n) printf("!=(%d)", nSites); //DEBUG

//   return n;
	return sites.size();
}

char* labCharToString(char label){
	switch (label){
	case 0:
		return (char*) "INIT";
		break;
	case 1:
		return (char*) "RET";
		break;
	case 2:
		return (char*) "CALL";
		break;
	case 3:
		return (char*) "JMP";
		break;
	case 4:
		return (char*) "END";
		break;
	case 5:
		return (char*) "SCALL";
		break;
	case 6:
		return (char*) "UREACH";
		break;
	case 7:
		return (char*) "UNDEF";
		break;
	case 8:
		return (char*) "JCC";
		break;
	case 9:
		//return "SEQ";
		return (char*) "INST";
		break;
	case 10:
		return (char*) "PATH";
		break;
	case 11:
		return (char*) "END";
		break;
	default:
		return (char*) "(UNKNOWN)";
	}
}

int sizePointer=12;
int sizeDotNode=8+3+2+15;
char* makeDotNode(void* n, int prof){
	char* s= (char*) malloc(sizeDotNode*sizeof(char));
	sprintf(s, "\"%p\" [ label = \"%d\" ]\n", n, prof);
	return s;
}

int sizeDotEdge=8+8+11+2+2+16+2+8;
char* makeDotEdge(void* pere, void* fils, int c1, int c2, int label, char withLabels){
	char* s= (char*) malloc(sizeDotEdge*sizeof(char));
	if (withLabels) sprintf(s, "\"%p\" -> \"%p\"[ label = \"%s: [%d %d]\"]\n", pere, fils, labCharToString(label), c1, c2);
	else sprintf(s, "\"%p\" -> \"%p\"[ label = \"[%d %d]\"]\n", pere, fils, c1, c2);
	return s;
}

char* drawDN(decisionNode* dn, int prof, char withLabels){
	char* s=makeDotNode(dn, prof);
	int i=0;

	if (dn->ht!=NULL){
		listDecisionNode* listdn=getChoices(dn->ht);
		char** strList=NULL;
		int nStr = 0;
		int tailleT=0;
		for (i=0; i<listdn->taille;i++){
			decisionNode* dnp=listdn->ldn[i];
			char* c=drawDN(dnp, prof+1, withLabels);
			tailleT+=strlen(c);
			nStr++;
			strList = (char**) realloc(strList, nStr*sizeof(char*));
			strList[nStr-1]=c;

			char* c2=makeDotEdge(dn, dnp, listdn->value1[i], listdn->value2[i], listdn->label[i], withLabels);
			tailleT+=strlen(c2);
			nStr++;
			strList = (char**) realloc(strList, nStr*sizeof(char*));
			strList[nStr-1]=c2;
		}

		char* ret= (char*) malloc(sizeDotNode+nStr*sizeDotEdge+tailleT);
		sprintf(ret, "%s\0", s);

		for (i=0; i<nStr;i++){
			strcat(ret, strList[i]);
		}
		return ret;
	}
	else {
		return s;
	}
}

char* drawDT(decisionTree* dt, char withLabels){
	return drawDN(dt->root, 0, withLabels);
}

void printInfoDT(decisionTree* dt){
	int nNodes=countNodesDT(dt);
	int nbMax=dt->nLearntSites*dt->siteSize+1;
	float rapportNode=100.-100*((float)nNodes)/((float)nbMax);
	float rapportSite=100.-100*((float)dt->nSites)/((float)dt->nLearntSites);
	printf("\nStats sur l'arbre de décision:\n");
	printf("%d sites appris de taille %d (max: %d nodes), %d sites uniques et %d nodes dans l'arbre, site compression: %.1f%%, node compression: %.1f%%\n", dt->nLearntSites, dt->siteSize, nbMax, dt->nSites, nNodes, rapportSite, rapportNode);
}
