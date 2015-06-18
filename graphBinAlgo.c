#include "graphBinAlgo.h"

char* Red="\e[1;31m";
char* Green="\e[1;32m";
char* Color_Off="\e[0m";
char optionMCSByPattern=0;
char optionMCS=0;
char optionNoPerm=0;
char optionCheckSymb=0;
char optionCount=0;
char optionRedInt=0;
char optionRec=0;
char optionQuiet=0;
char optionVerbose=0;
char optionOutSmall=0;
char optionExport=0;
char optionMultiThreaded=0;
char optionForceRoots=0;
char optionIsoOnly=0;
char optionDebug=0;
char optionInfo=0;
char optionOnlyInduced=0;
char optionLabels = 1;

FILE* FILEeP;
FILE* FILEeT;
FILE* FILEenP;
FILE* FILEenT;
FILE* FILEesP;
FILE* FILEesT;

sem_t mutexExport;
sem_t mutexIsoTotal;
sem_t mutexnSgraphP;
sem_t mutexStructRead;
sem_t mutexBSFPattern;
char* isThreadBusy;

int maxFound=0;
int maxPatternFound=0;
int isoTotal=0;
int wP=0;
int wT=0;
int nSgraphP=0;

int main(int argc, char* argv[]){
//	createGraphs();
//	exit(0);

	nThreads = 1;
	int i;
	char nbArgObl=0;

	FILE *fpPattern;
	char* pathPattern;
	FILE *fpToTest;
	char* pathToTest;

	for (i = 1; i < argc; i++){
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0){
			printf("Syntax : ./graphBinAlgo [options] arg1 [arg2]\n");
			printf("One argument : takes a .edg file and exports it as .dot, with reduction if -r or --reduction-interval option is specified.\nTwo arguments : First is the pattern graph, second is the graph to test.\n\n");

			printf("Options are :\n");
			printf("-i or --info : information about the given graph\n");
			printf("-mcs or --maximum-common-subgraph : search maximum subgraph and exports its number of nodes\n");
			printf("-bp or --mcs-by-pattern : search MCS from each node of the pattern graph\n");
			printf("-p number : number of nodes for BFS taken on pattern graph (not specified or 0 : whole graph)\n");
			printf("-t number : number of nodes for BFS taken on test graph (not specified or 0 : whole graph)\n");
			printf("-sm or --search-max : search maximum subgraph and exports its number of nodes");
			printf("-np or --no-permutation : forbids permutations in subgraph search (DEPRECATED)\n");
			printf("-cs or --check-symbols : checks if 2 nodes have same symbol before associating them\n");
      printf("-oi or --only-induced : only induced subgraphs are considered for isomorphism\n");
			printf("-c or --count : count the number of isomorphisms (doesn't stop on first found)\n");
			printf("-r or --reduction-interval : if one argument, reduces the input graph with the interval method\n");
			printf("-R or --recursive : if -redint, reduces it recursively until the graph is irreducible\n");
			printf("-e or --export : exports P roots matching in .eP file, T roots in .eT, P nodes in .enP and T nodes in .enT / (if isoo) graphSmall index for P in .esP, for T in .esT\n");
			printf("-m number or --multi-threaded number : works on multiples threads\n");
			printf("-fr or --force-root : forces roots to match\n");
			printf("-isoo or --iso-only : fast mode, uses Tarjan's method to compare same sized graphs\n");
			printf("-d or --debug : more debug messages\n");
			printf("-os or --output-small : outputs subgraphs for pattern and test graph, then exits\n");
			printf("-v or --verbose : verbose\n");
			printf("-q or --quiet : quiet\n");
			printf("-h or --help : displays this information\n");
			printf("\nExamples:\n./graphBinAlgo -p 24 -t 24 -cs graphPattern.edg graphToTest.edg\n");
			printf("./graphBinAlgo -p 24 -t 24 -cs --export graphPattern.edg graphToTest.edg\n");
			printf("./graphBinAlgo -p 24 -t 24 -cs -fr -m 16 graphPattern.edg graphToTest.edg\n");
			printf("./graphBinAlgo --check-symbols graphPattern.edg graphToTest.edg\n");

			return 0;
		}
		else if (strcmp(argv[i], "-p") == 0)
		{
			if (i + 1 < argc){
				i++;
				wP = atoi(argv[i]);
			}
			else {
				printf("Number of nodes for BFS on pattern graph not specified.\n");
			}
		}
		else if (strcmp(argv[i], "-t") == 0)
		{
			if (i + 1 < argc){
				i++;
				wT = atoi(argv[i]);
			}
			else {
				printf("Number of nodes for BFS on test graph not specified.\n");
			}
		}
		else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--info") == 0){
			optionInfo=1;
		}
		else if (strcmp(argv[i], "-mcs") == 0 || strcmp(argv[i], "--maximum-common-subgraph") == 0)
		{
			optionMCS=1;
		}
		else if (strcmp(argv[i], "-bp") == 0 || strcmp(argv[i], "--mcs-by-pattern") == 0)
		{
			optionMCSByPattern=1;
		}
		else if (strcmp(argv[i], "-np") == 0 || strcmp(argv[i], "--no-permutation") == 0)
		{
			optionNoPerm=1;
		}
		else if (strcmp(argv[i], "-cs") == 0 || strcmp(argv[i], "--check-symbols") == 0)
		{
			optionCheckSymb=1;
		}
    else if (strcmp(argv[i], "-oi") == 0 || strcmp(argv[i], "--only-induced") == 0)
    {
      optionOnlyInduced=1;
    }
		else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--export") == 0)
		{
			optionExport=1;
		}
		else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--count") == 0)
		{
			optionCount=1;
		}
		else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--reduction-interval") == 0)
		{
			optionRedInt=1;
		}
		else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0)
		{
			optionDebug=1;
		}
		else if (strcmp(argv[i], "-os") == 0 || strcmp(argv[i], "--output-small") == 0)
		{
			optionOutSmall=1;
		}
		else if (strcmp(argv[i], "-R") == 0 || strcmp(argv[i], "--recursive") == 0)
		{
			optionRec=1;
		}
		else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--multi-threaded") == 0)
		{
			optionMultiThreaded=1;

			if (i + 1 < argc){
				i++;
				nThreads = atoi(argv[i]);
			}
			else {
				printf("Need number of threads (-m 2 for instance).\n");
			}
		}
		else if (strcmp(argv[i], "-fr") == 0 || strcmp(argv[i], "--force-roots") == 0)
		{
			optionForceRoots=1;
		}
		else if (strcmp(argv[i], "-isoo") == 0 || strcmp(argv[i], "--iso-only") == 0)
		{
			optionIsoOnly=1;
		}
		else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
		{
			optionVerbose=1;
		}
		else if (strcmp(argv[i], "-q") == 0 || strcmp(argv[i], "--quiet") == 0)
		{
			optionQuiet=1;
		}
		else
		{
			if (nbArgObl == 0){
				fpPattern = fopen(argv[i], "r");

				if (fpPattern == NULL){
					printf("Can't open pattern graph %s\n", argv[i]);
					return 1;
				}
				pathPattern = argv[i];
				nbArgObl++;
			}
			else if (nbArgObl == 1){
				fpToTest = fopen(argv[i], "r");

				if (fpToTest == NULL){
					printf("Can't open test graph %s\n", argv[i]);
					return 1;
				}
				pathToTest = argv[i];
				nbArgObl++;
			}
		}
	}

	if (optionVerbose){
		printf("Options are : p %d, t %d, nThreads %d, ", wP, wT, nThreads);

		if (optionInfo) printf("Info ");
		if (optionMCS) printf("MCS ");
		if (optionMCSByPattern) printf("MCSByPattern ");
		if (optionNoPerm) printf("NoPerm ");
		if (optionCheckSymb) printf("CheckSymbols ");
		if (optionCount) printf("Count ");
		if (optionRedInt) printf("ReductionInterval ");
		if (optionRec) printf("Recursive ");
		if (optionMultiThreaded) printf("MultiThreaded ");
		if (optionForceRoots) printf("ForceRoots ");
		if (optionIsoOnly) printf("IsoOnly ");
		if (optionExport) printf("Export ");
		if (optionOutSmall) printf("OutSmall ");
		if (optionDebug) printf("Debug ");
		if (optionQuiet) printf("Quiet ");
		if (optionVerbose) printf("Verbose ");
		printf("\n");
	}

	if (wP != wT && optionIsoOnly){
		printf("The option IsoOnly is only available when p == t.\n");
		return 1;
	}

	if (nbArgObl <= 1){
		if (nbArgObl == 1){
			// input : a graphbin file, outputs :
			// a dot file if not verbose
			// information about the .edg if verbose

			if (is_graphbin_file(fpPattern)){
				graph_t* gr;
				graph_from_file(&gr, fpPattern, optionLabels);
				fclose(fpPattern);

				if (optionInfo){
					printf("File: %s\n", pathPattern);
					printf("%d nodes\n", gr->nodes.count);
					if (wP!=0){
					graph_t* graph;
					int k=0;
					for (i=0; i<gr->nodes.count; i++){
						graph=BFS_gba(gr, i, wP);

						if (graph != NULL){
							k++;
						}
						graph_free(graph);
					}
					printf("%d subgraphs of size %d\n", k, wP);
					}
					else{
						printf("Specify -p (number) option to know how many subgraphs it has.\n");
					}
				}
				else{
					if (optionRedInt==1){
						int i=0;
						int lastN=(&(gr->nodes))->count;
						int N=0;
						FILE* fout;
						long lenbase=strlen(pathPattern);
						char* pathTotal=calloc(256, sizeof(char));
						sprintf(pathTotal, "%s", pathPattern);

						printf("Reduction %d, n: %d\n", i, lastN);
						do{
							lastN=(&(gr->nodes))->count;
							gr=reduceInterval(gr);
							i++;
							N=(&(gr->nodes))->count;
							printf("Reduction %d, n: %d", i, N);

							if (lastN != N){
								printf("\n");
								lenbase=lenbase+5;
								char chemin2[lenbase];

								sprintf(chemin2, "%s%s", pathTotal, ".redd");
								fout = fopen(chemin2, "w");
								graph_fprint(fout, gr);
								fclose(fout);

								sprintf(chemin2, "%s%s", pathTotal, ".redg");
								fout = fopen(chemin2, "w");
								graph_to_file(gr, fout);
								fclose(fout);

								sprintf(pathTotal, "%s", chemin2);
							}
							else{
								printf(" max\n");
								break;
							}

						} while(optionRec);
					}
					else {
						long len=strlen(pathPattern)+4;
						char chemin[len];
						sprintf(chemin, "%s%s", pathPattern, ".dot");

						FILE *fpD;
						fpD = fopen(chemin, "w");
						graph_fprint(fpD, gr);
						fclose(fpD);
					}
				}
			}
			else {
				fclose(fpPattern);
			}
		}
		else {
			printf("One argument : takes a .edg file and exports it as .dot, with reduction if -r or --reduction-interval.\nTwo arguments : First is the pattern graph, second is the graph to test.\n./graphBinAlgo -h for more information.\n");
		}
		return 1;
	}
	else{
		if (is_graphbin_file(fpPattern) && is_graphbin_file(fpToTest)){
			graph_t* grPattern;
			char openP=graph_from_file(&grPattern, fpPattern, optionLabels);
			fclose(fpPattern);

			graph_t* grToTest;
			char openT=graph_from_file(&grToTest, fpToTest, optionLabels);
			fclose(fpToTest);

			if (openP!=0 || openT!=0){
				printf("T: %s (%d), ", pathToTest, grToTest->nodes.count);
				printf("P: %s (%d), ", pathPattern, grPattern->nodes.count);
				printf("One of the files is invalid (empty graph ?)\n");
//				return 1;
			}

			printf("P: %s (%d), ", pathPattern, grPattern->nodes.count);
			printf("T: %s (%d), ", pathToTest, grToTest->nodes.count);

			int nSgraphT=0;
			graph_t* vgraphP;
			graph_t* vgraphT;
			float res3=0;

			if (optionOutSmall){
				if (wT == 0 && wP == 0){
					printf("Export small graphs (BFS): must specify size (-p or -t options).\n");
					return 1;
				}

				graph_t* graph;
        if (wT != 0){
          for (i=0; i<grToTest->nodes.count; i++){
            graph=BFS_gba(grToTest, i, wT);

            if (graph != NULL){
              // outputs the test graph as .dot
              FILE *file;
              long len=14+4;
              char chemin[len];
              sprintf(chemin, "%s%d%s", "graphSmallT", i, ".dot");
              file = fopen(chemin, "w");
              graph_fprint(file, graph);
              fclose(file);

              // outputs the test graph as .edg
              FILE *file2;
              sprintf(chemin, "%s%d%s", "graphSmallT", i, ".edg");
              file2 = fopen(chemin, "w");
              graph_to_file(graph, file2);
              fclose(file2);
            }
            graph_free(graph);
          }
        }

        if (wP != 0){
          for (i=0; i<grPattern->nodes.count; i++){
            graph=BFS_gba(grPattern, i, wP);

            if (graph != NULL){
              // outputs the pattern graph as .dot
              FILE *file;
              long len=14+4;
              char chemin[len];
              sprintf(chemin, "%s%d%s", "graphSmallP", i, ".dot");
              file = fopen(chemin, "w");
              graph_fprint(file, graph);
              fclose(file);

              // outputs the pattern graph as .edg
              FILE *file2;
              sprintf(chemin, "%s%d%s", "graphSmallP", i, ".edg");
              file2 = fopen(chemin, "w");
              graph_to_file(graph, file2);
              fclose(file2);
            }
            graph_free(graph);
          }
        }

				return 0;
			}

			if (optionExport){
				FILEeP = fopen(".eP", "w");
				FILEeT = fopen(".eT", "w");
				FILEenP = fopen(".enP", "w");
				FILEenT = fopen(".enT", "w");
				FILEesP = fopen(".esP", "w");
				FILEesT = fopen(".esT", "w");
			}

			if (wT == 0){
				if (wP == 0){
					// the whole P graph against the whole T graph
					isoTotal=isoUllman(grPattern, grToTest);

					if (isoTotal>=1 && grPattern->nodes.count >=1 && grToTest->nodes.count >=1){
						printf("%sisoTotal: %d%s\n", Red, isoTotal, Color_Off);
					}
					else{
						//printf("%snot found%s\n", Green, Color_Off);
						printf("%sisoTotal: %d%s\n", Green, isoTotal, Color_Off);
					}
				}
				else {
          int iso;
					// wP != 0 so we need to use the BFS
					for (i=0; i<grPattern->nodes.count;i++){
						vgraphP=BFS_gba(grPattern, i, wP);
						if (vgraphP!=NULL){
              if (optionVerbose) printf("processing site %i\n", i);
							nSgraphP++;

							// tries and find subgraph isomorphisms with graphToTest :
              iso = isoUllman(vgraphP, grToTest);
							isoTotal += iso;
//               if (iso == 0) printf("site n°%d not found.\n", i);
						}
					}

					if (isoTotal>=1 && grPattern->nodes.count >=1 && grToTest->nodes.count >=1){
						if (nSgraphP != 0) res3 = (float) isoTotal / nSgraphP;
						printf("isoTotal: %d, nSgraphP: %d, %siso/nSgraphP: %f%s\n", isoTotal, nSgraphP, Red, res3, Color_Off);
					}
					else{
						//printf("%snot found%s\n", Green, Color_Off);
						printf("isoTotal: %d, nSgraphP: %d, %siso/nSgraphP: %d%s\n", 0, nSgraphP, Green, 0, Color_Off);
					}
				}
			}
			else {
				if (wP == 0){
					// wT != 0 so we need to use the BFS
					for (i=0; i<grToTest->nodes.count;i++){
						vgraphT=BFS_gba(grToTest, i, wT);
						if (vgraphT!=NULL){
							nSgraphT++;

							// tries and find subgraph isomorphisms with graphToTest :
							isoTotal+=isoUllman(grPattern, vgraphT);
						}
					}

					if (isoTotal>=1 && grPattern->nodes.count >=1 && grToTest->nodes.count >=1){
						if (nSgraphT != 0) res3 = (float) isoTotal / nSgraphT;
						printf("isoTotal: %d, nSgraphT: %d, %siso/nSgraphT: %f%s\n", isoTotal, nSgraphT, Red, res3, Color_Off);
					}
					else{
						//printf("%snot found%s\n", Green, Color_Off);
						printf("isoTotal: %d, nSgraphT: %d, %siso/nSgraphT: %d%s\n", 0, nSgraphT, Green, 0, Color_Off);
					}
				}
				else {
					if (!optionIsoOnly){
						// first we generate all the subgraphs
						graph_t** vgraphsT = malloc(grToTest->nodes.count * sizeof(graph_t*));
						sem_init(&mutexExport, 0, 1);
						sem_init(&mutexIsoTotal, 0, 1);
						sem_init(&mutexnSgraphP, 0, 1);
						sem_init(&mutexStructRead, 0, 1);
						sem_init(&mutexBSFPattern, 0, 1);
						pthread_t thread[nThreads];
						int k;

						for (i=0; i<grToTest->nodes.count; i++){
							vgraphsT[i]=BFS_gba(grToTest, i, wT);
//							graph_t* g = vgraphsT[i];
						}

						int borneThread[nThreads+1];
						for (k=0; k<nThreads+1; k++){
							borneThread[k]=(k*grPattern->nodes.count)/nThreads;
						}

						for (k=0; k<nThreads; k++){
								sem_wait(&mutexStructRead);
								struct structProcessGraph grData = { grPattern, borneThread[k], borneThread[k+1], grToTest->nodes.count, vgraphsT };
								pthread_create(&thread[k], NULL, processSubGraphs, &grData);
						}

						void* retour;

						for (k=0; k<nThreads; k++){
							pthread_join(thread[k], &retour);

							if (retour != PTHREAD_CANCELED){
								int* tabInt;
								tabInt=retour;
								nSgraphP+=tabInt[0];
								isoTotal+=tabInt[1];
							}
							else {
								printf("Thread cancelled\n");
							}
						}

						for (i=0; i<grToTest->nodes.count; i++){
							graph_free(vgraphsT[i]);
						}

						if (isoTotal>=1 && grPattern->nodes.count >=1 && grToTest->nodes.count >=1){
							if (nSgraphP != 0) res3 = (float) isoTotal / nSgraphP;
							printf("isoTotal: %d, nSgraphP: %d, %siso/nSgraphP: %f%s\n", isoTotal, nSgraphP, Red, res3, Color_Off);
						}
						else{
//							printf("%snot found%s\n", Green, Color_Off);
							printf("isoTotal: %d, nSgraphP: %d, %siso/nSgraphP: %d%s\n", 0, nSgraphP, Green, 0, Color_Off);
						}
					}
					else{
						// first we generate all the subgraphs
						char** mT = malloc(grToTest->nodes.count * sizeof(char*));
						char** mP = malloc(grPattern->nodes.count * sizeof(char*));

						for (i=0; i<grPattern->nodes.count; i++){
							mP[i]=BFS2(grPattern, i, wP);
						}

						for (i=0; i<grToTest->nodes.count; i++){
							mT[i]=BFS2(grToTest, i, wT);

							if (mT[i] != NULL){
								nSgraphT++;
							}
//							printf("%d\n", i);
						}

						int j;
						for (i=0; i<grPattern->nodes.count; i++){
							if (mP[i] != NULL){
								nSgraphP++;
								for (j=0; j<grToTest->nodes.count; j++){
									if (mT[j] != NULL){
										if (optionDebug){
											printf("\nDEBUG ");
											printf("P %d, T %d ", i, j);
										}
										// compares mP[i] to mT[j]
										if (pcharcmp(mP[i], mT[j], wP*wP)){
											if (optionDebug){
												printf("Match");
											}

											if (optionExport){
												// exports T graphSmall index
												fprintf(FILEesT, "%d\n", j);
												fflush(FILEesT);

												// exports P graphSmall index
												fprintf(FILEesP, "%d\n", i);
												fflush(FILEesP);
											}

											isoTotal++;
											break;
										}
									}
								}

								if (optionDebug  && j==grToTest->nodes.count) printf("\nDEBUG %d No", i);
							}
						}

						free(mP);
						free(mT);

						if (isoTotal>=1 && grPattern->nodes.count >=1 && grToTest->nodes.count >=1){
							if (nSgraphP != 0) res3 = (float) isoTotal / nSgraphP;
							printf("isoTotal: %d, nSgraphP: %d, nSgraphT: %d, %siso/nSgraphP: %f%s\n", isoTotal, nSgraphP, nSgraphT, Red, res3, Color_Off);
						}
						else{
							printf("isoTotal: %d, nSgraphP: %d, nSgraphT: %d, %siso/nSgraphP: %d%s\n", 0, nSgraphP, nSgraphT, Green, 0, Color_Off);
//							printf("%snot found%s\n", Green, Color_Off);
						}

					}
				}
			}

			if (optionExport){
				fclose(FILEeP);
				fclose(FILEeT);
				fclose(FILEenP);
				fclose(FILEenT);
				fclose(FILEesP);
				fclose(FILEesT);
			}

			// There is a segfault when freeing a graph with 0 node
			// TODO : fix it in graph_free
			if (grPattern->nodes.count >=1) graph_free(grPattern);
			if (grToTest->nodes.count >=1) graph_free(grToTest);
		}
		else{
			printf("One of theses files is not a binary dump of a graph.\n");
			fclose(fpPattern);
			fclose(fpToTest);
			return 1;
		}
	}

	return 0;
}