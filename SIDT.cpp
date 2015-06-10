#include "SIDT.h"

char optionFuncs=0;

void printUsage(){
	printf("Use SIDT (Site Identification Decision Tree) to learn and scan sites in .edg files.\n");
	printf("Usage : ./SIDT [options] [command]\n");
	printf("Commands are :\n");
	printf("        -h or --help : print this help\n");
	printf("        -l or --learn fileToLearn [fileToScan] : learns a .edg file [and scans a .edg file]\n");
	printf("        -ll or --learn-list listToLearn [listToScan] : learns a list of .edg files and scans a list of .edg files\n");
	printf("A list of .edg files has a path to a .edg file per line, for instance :\n");
	printf("        firefox.edg\n");
	printf("        ../samples/sality.edg\n");
	printf("        StuxDuqu/duqu.edg\n");
	printf("Options are :\n");
	printf("        -ncl or --no-check-labels : do not check the symbols (labels) of sites\n");
	printf("        -ss or --site-size siteSize : specifies the wanted size of the sites (default: 24)\n");
	printf("        -s or --stats : prints stats about the decision tree\n");
	printf("        -o or --output-dt [path] : outputs the decision tree as a .dot file for display\n");
	printf("        -lc or --limit-site-count limit : specifies how many sites should be learnt in total (for debug purposes, default: unlimited)\n");
	printf("Examples :\n");
	printf("        ./SIDT -ss 16 -l firefox.edg firefox.edg : learns and scans a single file with sites of size 16\n");
	printf("        ./SIDT  -lc 30 -o DTfirefox.dot -l firefox.edg : learns only 30 sites from a single file and outputs the decision tree as a .dot file\n");
	printf("        ./SIDT -s -ll malwares.lst sample.lst : learns a list of malwares and scans a list of files, then prints stats about the decision tree\n");
}

int main(int argc, char* argv[]){
	if (argc <= 2){
		printUsage();
		return 0;
	}

	FILE *fpPattern=NULL;
	char* pathPattern;
	FILE *fpTest=NULL;
	char* pathTest;
	char optionLearn=0;
	char optionLearnList=0;
	char learnOk=0;
	char scanOk=0;
	char optionTest=0;
	char optionShowStats=0;
	char optionOutputDt=0;
	char* pathDT;
	int siteCountLimit=0;
	int siteSize=24;
	char withLabels=1;
  bool optionInfo = false;

	ProgInfo* scanInfo=NULL;

	int a;
	for (a = 1; a < argc; a++){
		if (strcmp(argv[a], "-h") == 0 || strcmp(argv[a], "-help") == 0 || strcmp(argv[a], "--help") == 0){
			printUsage();
			return 0;
		}
		else if (strcmp(argv[a], "-l") == 0 || strcmp(argv[a], "--learn") == 0)
		{
			optionLearn=1;
		}
		else if (strcmp(argv[a], "-ll") == 0 || strcmp(argv[a], "--learn-list") == 0){
			optionLearnList=1;
		}
		else if (strcmp(argv[a], "-s") == 0 || strcmp(argv[a], "--stats") == 0){
			optionShowStats=1;
		}
		else if (strcmp(argv[a], "-ncl") == 0 || strcmp(argv[a], "-ncs") == 0 || strcmp(argv[a], "--no-check-labels") == 0){
			withLabels=0;
		}
		else if (strcmp(argv[a], "-o") == 0 || strcmp(argv[a], "--output-dt") == 0){
			optionOutputDt=1;
			if (a<argc-1){
				pathDT=argv[a+1];
				a++;
			}
			else {
				printf("No file specified to output the DT, default is dt_output.dot.\n");
				pathDT=(char*) "dt_output.dot";
			}
		}
		else if (strcmp(argv[a], "-lc") == 0 || strcmp(argv[a], "--limit-site-count") == 0){
			if (a<argc-1){
				siteCountLimit=atoi(argv[a+1]);
				a++;
			}
			else {
				printf("Need number to limit site count\n");
				printUsage();
				return 1;
			}
		}
		else if (strcmp(argv[a], "-ss") == 0 || strcmp(argv[a], "--site-size") == 0){
			if (a<argc-1){
				siteSize=atoi(argv[a+1]);
				a++;
			}
			else {
				printf("Need number to specify site size\n");
				printUsage();
				return 1;
			}
		}
		else
		{
			if (!learnOk){
				fpPattern = fopen(argv[a], "r");

				if (fpPattern == NULL){
					printf("Can't open pattern graph %s\n", argv[a]);
					return 1;
				}
				pathPattern = argv[a];
				learnOk=1;
			}
			else if (!scanOk) {
				optionTest=1;
				fpTest = fopen(argv[a], "r");

				if (fpTest == NULL){
					printf("Can't open test graph %s\n", argv[a]);
					return 1;
				}
				pathTest = argv[a];
				scanOk=1;
			}

			else {
				optionFuncs=1;
				scanInfo=progFuncFromFile(argv[a]);
//				printf("%p %p %p\n", scanInfo, scanInfo->functions, *scanInfo->functions[0]->name);
//				return 0;
			}
		}
	}

	if (fpPattern==NULL){
		printf("Not file to learn.\n");
		printUsage();
		return 1;
	}

	char valence=2;
	int n=0;
  int n_pattern = 0;
  int n_test = 0;
	decisionTree* dt=newDecisionTree(siteSize, valence);

	if (optionLearn){
		graph_t* gr;
		graph_from_file(&gr, fpPattern);
		fclose(fpPattern);

		char* msg= (char*) malloc((strlen(pathPattern)+1)*sizeof(char));
		sprintf(msg, "%s", pathPattern);
		n_pattern = learnGraph(gr, dt, msg, siteCountLimit, withLabels);
		printf("%d sites learned from %s\n", n_pattern, msg);
		graph_free(gr);

		if (optionTest){
			graph_from_file(&gr, fpTest);
			fclose(fpTest);

			int* countByProg= (int*) calloc(dt->nProgs, sizeof(int));
			int* scanFuncs;
			if (optionFuncs) scanFuncs= (int*) calloc(scanInfo->n, sizeof(int));
			n=findGraph(gr, dt, countByProg, scanInfo, scanFuncs, withLabels);
			printf("%d sites of %s found from %s\n", n, pathTest, dt->progs[0]->message);
      printf("Pattern graph %s has %d sites ; test graph %s has %d sites found matching.\n", pathPattern, n_pattern, pathTest, n);
			if (optionFuncs){
				printf("\nFunctions found: \n");
				int k;
				for (k=0; k<scanInfo->n; k++){
					if (scanFuncs[k]!=0){
						printf("%d %s\n", scanFuncs[k], scanInfo->functions[k]->name);
					}
				}
			}
			if (optionFuncs) free(scanFuncs);
			free(countByProg);
			graph_free(gr);
		}
	}
	else if (optionLearnList){
		// learning :
		if (optionInfo) printf("Learning :\n");
		char line[128];
		FILE* fp;
		graph_t* gr;
		int i=0;
		while (fgets(line, sizeof(line), fpPattern) != NULL && (siteCountLimit == 0 || dt->nLearntSites < siteCountLimit)){
//       if (siteCountLimit == 0 || siteCountLimit < dt->nLearntSites){
			i++;
			line[strlen(line)-1]=0;
			fp=fopen(line, "r");
			if (fp == NULL){
				if (optionInfo) fprintf(stderr, "%d, ERROR: Can't open graph to learn %s\n", i, line);
				continue;
			}

			char a=graph_from_file(&gr, fp);
			fclose(fp);
			if (a!=0){
				if (optionInfo) fprintf(stderr, "%d, This file is not a valid graph: %s\n", i, line);
				continue;
			}

			char* msg= (char*) malloc((strlen(line)+1)*sizeof(char));
			sprintf(msg, "%s", line);
			n=learnGraph(gr, dt, msg, siteCountLimit, withLabels);
			if (optionInfo) printf("%d, %s: %d sites learned\n", i, msg, n);
			graph_free(gr);
//       }
		}
		fclose(fpPattern);
//		read();
//		scanf();
    
    if (optionInfo) printf("%d unique sites.\n", dt->nLearntSites);

		if (optionTest){
			// scanning :
			if (optionInfo) printf("\nDetection :\n");
			char line[128];
			i=0;
			int k=0;
			while (fgets(line, sizeof(line), fpTest) != NULL){
				i++;
				line[strlen(line)-1]=0;
				fp=fopen(line, "r");
				if (fp == NULL){
					if (optionInfo) fprintf(stderr, "%d, ERROR: Can't open graph to test %s\n", i, line);
					continue;
				}

				char a=graph_from_file(&gr, fp);
				fclose(fp);
				if (a!=0){
					if (optionInfo) fprintf(stderr, "%d, This file is not a valid graph: %s\n", i, line);
					continue;
				}

				int* countByProg = (int*) calloc(dt->nProgs, sizeof(int));
				int* scanFuncs;
				if (optionFuncs) scanFuncs = (int*) calloc(scanInfo->n, sizeof(int));
				n=findGraph(gr, dt, countByProg, scanInfo, scanFuncs, withLabels);
				if (optionInfo) printf("%d, %s: %d sites found\n", i, line, n);
				for (k=0; k<dt->nProgs; k++){
					if (countByProg[k]!=0){ //TODO >=30 sinon faux positif ??
						printf("   %d from %s (%d sites)\n", countByProg[k], dt->progs[k]->message, dt->progs[k]->nSites);
					}
				}

				if (optionFuncs){
					if (optionInfo) printf("\nFunctions found: \n");
					for (k=0; k<scanInfo->n; k++){
						if (scanFuncs[k]!=0){
							printf("%d %s\n", scanFuncs[k], scanInfo->functions[k]->name);
						}
					}
				}

				if (optionFuncs) free(scanFuncs);
				free(countByProg);
				graph_free(gr);
			}
			fclose(fpTest);
		}
	}

	if (optionOutputDt){
		char* dotstr=drawDT(dt, withLabels);
		FILE* fpp=fopen(pathDT, "w");
		if (fpp == NULL){
			fprintf(stderr, "Cannot open %s\n", pathDT);
		}
		fprintf(fpp, "Digraph {\n%s}", dotstr);
		fclose(fpp);
	}

	if (optionShowStats){
		printInfoDT(dt);
	}

//	while(0==0){
//
//	}

	if (optionFuncs) freeProgFunctions(scanInfo);
	if (optionInfo) freeDecisionTree(dt); // TODO: à enlever, c'est juste pour les tests en performance
}

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
