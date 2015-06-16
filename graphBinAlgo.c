/*
 ============================================================================
 Name        : graphBinAlgo.c
 Author      : Aurelien Thierry
 Version     :
 Copyright   : Your copyright notice
 Description : Tests of algorithms for graph isomorphisms on binary dumps
 ============================================================================
 */

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

struct structProcessGraph{
   graph_t* grPattern;
   vsize_t debutPattern;
   vsize_t finPattern;
   vsize_t tailleTest;
   graph_t** graphsToTest;
};

int main(int argc, char* argv[]){
//	createGraphs();
//	exit(0);

	int nThreads=1;
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
				graph_from_file(&gr, fpPattern);
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
			char openP=graph_from_file(&grPattern, fpPattern);
			fclose(fpPattern);

			graph_t* grToTest;
			char openT=graph_from_file(&grToTest, fpToTest);
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

int pcharcmp(char* c1, char* c2, int size){
	char eq=1;
	int i;

	for (i=0; i<size; i++){
		if (c1[i] != c2[i]){
			eq=0;
			break;
		}
	}

	return eq;
}

void* processSubGraphs(void* grData){
	struct structProcessGraph *d = grData;
	graph_t* grPattern=d->grPattern;
	graph_t** vgraphsT=d->graphsToTest;
	size_t debutPattern=d->debutPattern;
	size_t finPattern=d->finPattern;
	size_t tailleTest=d->tailleTest;
	sem_post(&mutexStructRead);

	if (optionDebug) printf("I will process %d tests graphs, taking pattern graphs between %d and %d.\n", tailleTest, debutPattern, finPattern);
	int i;
	int j;
	int isoLocal=0;
	int nSgraphPLocal=0;
	graph_t* vgraphP;
	for (j=debutPattern; j<finPattern; j++){
		if (optionDebug) printf("processing pattern %d\n", j);
		sem_wait(&mutexBSFPattern);
		vgraphP=BFS_gba(grPattern, j, wP);
		sem_post(&mutexBSFPattern);
		if (vgraphP != NULL){
			nSgraphPLocal++;

			for (i=0; i<tailleTest; i++){
				if (optionDebug) printf("Pattern %d against Test %i\n", j, i);
				if (vgraphsT[i] != NULL){
					if (optionDebug){
						printf("\nDEBUG P %d, T %d ", j, i);
					}

					int nbIso=isoUllman(vgraphP, vgraphsT[i]);
					isoLocal+=nbIso;

					if (nbIso != 0){
						if (optionDebug){
							printf("Match");
						}

						if (optionExport){
							sem_wait(&mutexExport);
							// exports T roots
							fprintf(FILEeT, "%x\n", (int) vgraphsT[i]->root->node_id);
							fflush(FILEeT);

							// exports P roots
							fprintf(FILEeP, "%x\n", (int) grPattern->root->node_id);
							fflush(FILEeP);

							// exports all T nodes
							node_list_t* listG = &(vgraphsT[i]->nodes);

							int i;
							for (i=0; i<listG->count; i++){
								node_t* n = node_list_item(listG, i);
								fprintf(FILEenT, "%x ", (int) n->node_id);
							}

							fprintf(FILEenT, "\n");
							fflush(FILEenT);

							// exports all P nodes
							listG = &(vgraphP->nodes);

							for (i=0; i<listG->count; i++){
								node_t* n = node_list_item(listG, i);
								fprintf(FILEenP, "%x ", (int) n->node_id);
							}

							fprintf(FILEenP, "\n");
							fflush(FILEenP);
							sem_post(&mutexExport);
						}

						break;
					}
				}
			}

			if (optionDebug && i==tailleTest && !optionMultiThreaded){
				printf("\nDEBUG P %d No", j);
			}
		}
		graph_free(vgraphP);
	}

	int* tabInt = malloc(2*sizeof(int));
	tabInt[0]=nSgraphPLocal;
	tabInt[1]=isoLocal;
	pthread_exit ((void *) tabInt);
}

void createGraphs(){
	// Creates a graph pattern for tests (simple graph with 3 nodes)
	int nP=3;
	graph_t* grPattern = graph_alloc(nP);

	node_t* nP0=node_list_append(&grPattern->nodes, 0);
	node_t* nP1=node_list_append(&grPattern->nodes, 1);
	node_t* nP2=node_list_append(&grPattern->nodes, 2);
//	node_t* nP3=node_list_append(&grPattern->nodes, 3);

	nP0->symb=8;
	nP1->symb=8;
	nP2->symb=9;

	int i;
	for (i=4; i<nP; i++){
		node_list_append(&grPattern->nodes, i);
	}

	node_link(nP0, nP1);
	node_link(nP0, nP2);
//	node_link(nP2, nP0);

	grPattern->root=nP0;

	// outputs the pattern as .dot
	FILE *fpP;
	fpP = fopen("graphPattern.dot", "w");
	graph_fprint(fpP, grPattern);
	fclose(fpP);

	// outputs the pattern as .graphbin (binary dump)
	FILE *fpPb;
	fpPb = fopen("graphPattern.graphbin", "w");
	graph_to_file(grPattern, fpPb);
	fclose(fpPb);

	graph_free(grPattern);

	// Creates a (simple) graph containing the pattern with 4 nodes
	int nT=10;
	graph_t* grToTest = graph_alloc(nT);

	node_t* nT0=node_list_append(&grToTest->nodes, 0);
	node_t* nT1=node_list_append(&grToTest->nodes, 1);
	node_t* nT2=node_list_append(&grToTest->nodes, 2);
	node_t* nT3=node_list_append(&grToTest->nodes, 3);
	node_t* nT4=node_list_append(&grToTest->nodes, 4);
	node_t* nT5=node_list_append(&grToTest->nodes, 5);
	node_t* nT6=node_list_append(&grToTest->nodes, 6);
	node_t* nT7=node_list_append(&grToTest->nodes, 7);
	node_t* nT8=node_list_append(&grToTest->nodes, 8);
	node_t* nT9=node_list_append(&grToTest->nodes, 9);

	nT0->symb=8;
	nT1->symb=8;
	nT2->symb=8;
	nT3->symb=9;
	nT4->symb=9;
	nT5->symb=8;
	nT6->symb=8;

	for (i=10; i<nT; i++){
		node_list_append(&grToTest->nodes, i);
	}

	node_link(nT0, nT1);
	node_link(nT1, nT2);
	node_link(nT2, nT0);
	node_link(nT1, nT3);
	node_link(nT2, nT3);
	node_link(nT2, nT5);
	node_link(nT2, nT6);
	node_link(nT3, nT5);
	node_link(nT6, nT5);
	node_link(nT6, nT8);
	node_link(nT5, nT9);
	node_link(nT8, nT9);

	grToTest->root=nT1;

	// outputs the test graph as .dot
	FILE *fpT;
	fpT = fopen("graphToTest.dot", "w");
	graph_fprint(fpT, grToTest);
	fclose(fpT);

	// outputs the test graph as .graphbin (binary dump)
	FILE *fpTb;
	fpTb = fopen("graphToTest.graphbin", "w");
	graph_to_file(grToTest, fpTb);
	fclose(fpTb);

	graph_free(grToTest);


	// Creates a (simple) graph for testings with permutations
	int nPermP=7;
	graph_t* grPermP = graph_alloc(nPermP);

	nT0=node_list_append(&grPermP->nodes, 0);
	nT1=node_list_append(&grPermP->nodes, 1);
	nT2=node_list_append(&grPermP->nodes, 2);
	nT3=node_list_append(&grPermP->nodes, 3);
	nT4=node_list_append(&grPermP->nodes, 4);
	nT5=node_list_append(&grPermP->nodes, 5);
	nT6=node_list_append(&grPermP->nodes, 6);

	nT0->symb=8;
	nT1->symb=8;
	nT2->symb=8;
	nT3->symb=9;
	nT4->symb=9;//2
	nT5->symb=9;
	nT6->symb=9;//1

	node_link(nT0, nT1);
	node_link(nT0, nT2);
	node_link(nT1, nT3);
	node_link(nT1, nT4);
	node_link(nT2, nT5);
	node_link(nT2, nT6);


	grPermP->root=nT0;

	// outputs the test graph as .dot
	fpT = fopen("graphPermP.dot", "w");
	graph_fprint(fpT, grPermP);
	fclose(fpT);

	// outputs the test graph as .graphbin (binary dump)
	fpTb = fopen("graphPermP.graphbin", "w");
	graph_to_file(grPermP, fpTb);
	fclose(fpTb);

	graph_free(grPermP);

	// Creates a (simple) graph for testings with permutations
	int nPermT=13;
	graph_t* grPermT = graph_alloc(nPermT);

	nT0=node_list_append(&grPermT->nodes, 0);
	nT1=node_list_append(&grPermT->nodes, 1);
	nT2=node_list_append(&grPermT->nodes, 2);
	nT3=node_list_append(&grPermT->nodes, 3);
	nT4=node_list_append(&grPermT->nodes, 4);
	nT5=node_list_append(&grPermT->nodes, 5);
	nT6=node_list_append(&grPermT->nodes, 6);
	nT7=node_list_append(&grPermT->nodes, 7);
	nT8=node_list_append(&grPermT->nodes, 8);
	nT9=node_list_append(&grPermT->nodes, 9);
	node_t* nT10=node_list_append(&grPermT->nodes, 10);
	node_t* nT11=node_list_append(&grPermT->nodes, 11);
	node_t* nT12=node_list_append(&grPermT->nodes, 12);

	nT0->symb=8;
	nT1->symb=8;
	nT2->symb=8;
	nT3->symb=9;
	nT4->symb=9;//2
	nT5->symb=9;
	nT6->symb=9;//1
	nT7->symb=9;
	nT8->symb=9;
	nT9->symb=8;
	nT10->symb=9;
	nT11->symb=9;
	nT12->symb=9;

	node_link(nT0, nT1);
	node_link(nT0, nT2);
	node_link(nT1, nT3);
	node_link(nT1, nT4);
	node_link(nT2, nT5);
	node_link(nT2, nT6);

	node_link(nT1, nT7);
	node_link(nT1, nT8);
	node_link(nT0, nT9);
	node_link(nT9, nT10);
	node_link(nT9, nT11);
	node_link(nT6, nT12);


	grPermT->root=nT0;

	// outputs the test graph as .dot
	fpT = fopen("graphPermT.dot", "w");
	graph_fprint(fpT, grPermT);
	fclose(fpT);

	// outputs the test graph as .graphbin (binary dump)
	fpTb = fopen("graphPermT.graphbin", "w");
	graph_to_file(grPermT, fpTb);
	fclose(fpTb);

	graph_free(grPermT);

}

vsize_t min(vsize_t a, vsize_t b){
	if (a<b) return a; else return b;
}

int isoUllman(graph_t* grPattern, graph_t* grToTest){
	//Initializing M0:
	vsize_t nToTest = grToTest->nodes.count;
	vsize_t nPattern = grPattern->nodes.count;

//	printf("nP: %d, nT: %d\n", nPattern, nToTest);

	if ((!optionMCS && nToTest < nPattern) || nToTest == 0 || nPattern == 0){
		// Pattern is larger that the graph to test
		return 0;
	}

	char* M0=calloc(nToTest*nPattern, sizeof(char)); //initializes them to 0
	node_list_t* listT = &(grToTest->nodes);
	node_list_t* listP = &(grPattern->nodes);
	vsize_t i=0;
	vsize_t j=0;

	for(i=0; i<nToTest; i++){
		node_t* nTi = node_list_item(listT, i);

//		printf("i:%d, id: %d\n", i, (int) nTi->listid);
//		if (i != nTi->listid) printf("Oops.");

		vsize_t nTiIn=nTi->fathers_nb;
		vsize_t nTiOut=nTi->children_nb;


		for(j=0; j<nPattern; j++){
			node_t* nPj = node_list_item(listP, j);
			vsize_t nPjIn=nPj->fathers_nb;
			vsize_t nPjOut=nPj->children_nb;

//			printf("M: j=%d %d, i=%d %d\n", j+1, (int) nPj->symb, i+1, (int) nTi->symb);
//			printf("%d >= %d && %d >= %d?\n", nTiIn, nPjIn, nTiOut, nPjOut);
//
//			printf("M: j=%d, (%d, %d)\n", j+1, nPjIn, nPjOut);
//			printf("M: i=%d, (%d, %d)\n", i+1, nTiIn, nTiOut);

			// nPj->symb == nTi->symb : is it clever with reductions ? results might be different (slightly without reductions)
			// but it increases (a lot) performances
			if ((!optionCheckSymb || nPj->symb == nTi->symb) && (0==0 || optionMCS || (nTiIn >= nPjIn && nTiOut >= nPjOut))){
				M0[i*nPattern+j]=1;

				if (optionForceRoots){
					char c1 = nPj==grPattern->root;
					char c2 = nTi==grToTest->root;

					if ((c1 || c2) && (!c1 || !c2)){
						M0[i*nPattern+j]=0;
					}
				}
			}
		}
	}

	char assignedPattern[nPattern];
	char assignedToTest[nToTest];

	for (i=0; i<nPattern; i++){
		assignedPattern[i]=1; //0 assigned, 1 not assigned, unreachable, 2+ not assigned, reachable
	}

	for (i=0; i<nToTest; i++){
		assignedToTest[i]=1;
	}

	//Initializing F
	vsize_t F[2*nPattern];
	vsize_t Fmax=0;

	if (optionMCS) maxFound=0;
	int ret = backtrack(grPattern, grToTest, M0, 0, nPattern, nToTest, min(nPattern, nToTest), F, Fmax, assignedPattern, assignedToTest, 0, listP, listT, optionOnlyInduced);
	if (optionMCS) printf("MaxFound: %d, ", maxFound);

	free(M0);

	return ret;
}

void debugPrintF(vsize_t* F, vsize_t Fmax){
//	printf("F: j -- i\n");
	int fm;
	for(fm=0; fm<Fmax; fm+=2){
		printf("%d -- %d, ", F[fm+1], F[fm]);
	}
	printf("\n");
}

void debugPrintM(char* M0, vsize_t nPattern, vsize_t nToTest){
	printf("M:\n");
	int i, j;
	for (i=0; i<nToTest;i++){
		for (j=0; j<nPattern;j++){
			printf("%d", M0[i*nPattern+j]);
			printf(" ");
		}
		printf("\n");
	}
}

void debugPrintAssigned(char* assignedPattern, char* assignedToTest, vsize_t nPattern, vsize_t nToTest){
	vsize_t k;

	printf("assignedP :");
	for (k=0; k<nPattern; k++){
		printf("%d ", assignedPattern[k]);
	}
	printf("\n");

	printf("assignedT :");
	for (k=0; k<nToTest; k++){
		printf("%d ", assignedToTest[k]);
	}
	printf("\n");
}

int backtrack(graph_t* grPattern, graph_t* grToTest, char* M0, vsize_t j, vsize_t nPattern, vsize_t nToTest, vsize_t limit, vsize_t* F, vsize_t Fmax, char* assignedPattern, char* assignedToTest, vsize_t lastAssignedPatternPlusOne, node_list_t* listP, node_list_t* listT, char optionOnlyInduced){
	int ret=0;

// 	if (optionDebug) printf("processing j=%d\n", j);
// 	fflush(stdout);

	if (j>=limit){ //there is isomorphism : output it and move on (return)
//		debugPrintF(F, Fmax);
		return 1;
	}
	else{
		int i;
		int lmax;
		int lmin;
		// in case of search of MCS, every node in graphPattern will not necessarily be matched, so we need to test each of them
		if (optionMCS) lmax=nPattern; else lmax=1;
		if (optionMCS) lmin=0; else lmin=0;//lastAssignedPatternPlusOne
		int l;

		for (l=lmin; l<lmax; l++){
			if (j==0) maxPatternFound=0;

			for(i=0; i<nToTest;i++){
				if (!optionMCS) l=j;

//				if (l==12 && i==104){
//					printf("12104d\n");
//					debugPrintF(F, Fmax);
//					debugPrintAssigned(assignedPattern, assignedToTest, nPattern, nToTest);
//					printf("aP:%d, aT:%d, M:%d\n", assignedPattern[l], assignedToTest[i], M0[i*nPattern + l]);
//					printf("12104f\n");
//				}

				if ((!optionMCS || j==0 || (assignedPattern[l]>=2 && assignedToTest[i]>=2)) && M0[i*nPattern + l]==1){ // forearch i in G , Mij=1 :
					if (optionMCS && maxFound < j+1) {
						maxFound = j+1;
						if (optionVerbose) printf("maxFound: %d\n", maxFound);
					}
					if (optionMCSByPattern && maxPatternFound < j+1) {
						maxPatternFound = j+1;
//							if (optionVerbose) printf("Node l=%d, maxPatternFound: %d\n",l, maxPatternFound);
					}

					// F=F (Union) {(i,j)}
					F[Fmax]=i;
					F[Fmax+1]=l;
					Fmax+=2;

					// Update M -> M'
					char* Mp=malloc(nToTest*nPattern*sizeof(char));
					memcpy(Mp, M0, nPattern*nToTest);

					node_t* nPl;
					node_t* nTi;
					nPl = node_list_item(listP, l);
					nTi = node_list_item(listT, i);

					int k;
					if (optionMCS){
						for (k=0; k<nPl->children_nb; k++){
							vsize_t s = nPl->children[k]->list_id;
							if (assignedPattern[s]!=0) assignedPattern[s]++;
						}

						for (k=0; k<nTi->children_nb; k++){
							vsize_t s = nTi->children[k]->list_id;
							if (assignedToTest[s]!=0) assignedToTest[s]++;
						}
					}

//					printf("l: %d, i: %d\n", l, i);

					// Other patterns can't associate with i anymore :

					for(k=0; k<=nPattern - 1; k++){
						if (k!=l) Mp[i*nPattern+k]=0;
					}

					// Other toTest can't associate with l anymore
					for(k=0; k<=nToTest - 1; k++){
						if (k!=i) Mp[k*nPattern+l]=0;
					}

					assignedPattern[l]=0;
					assignedToTest[i]=0;

					// forbids permutations :
					if (optionNoPerm){
						forbidPerm(grPattern, grToTest, Mp, l, nPattern, nToTest, F, i);
					}

//					debugPrintF(F, Fmax);
//					debugPrintAssigned(assignedPattern, assignedToTest, nPattern, nToTest);
//					if (l==14 && i==103) debugPrintM(Mp, nPattern, nToTest);
	//				printf("j: %d\n", j);

					if (j+1>=limit || forwardChecking(grPattern, grToTest, Mp, l, nPattern, nToTest, F, Fmax, assignedPattern, assignedToTest, listP, listT, optionOnlyInduced)) {
// 						printf("forward\n");
	//					printf("p%d ", j);
						ret+=backtrack(grPattern, grToTest, Mp, j+1, nPattern, nToTest, limit, F, Fmax, assignedPattern, assignedToTest, l+1, listP, listT, optionOnlyInduced);
						if (ret != 0 && !optionCount) return ret;
					}

					free(Mp);

					// F=F - {(i,j)}
					Fmax-=2;

					if (optionMCS){
						for (k=0; k<nPl->children_nb; k++){
							vsize_t s = nPl->children[k]->list_id;
							if (assignedPattern[s]!=1) assignedPattern[s]--;
						}

						for (k=0; k<nTi->children_nb; k++){
							vsize_t s = nTi->children[k]->list_id;
							if (assignedToTest[s]!=1) assignedToTest[s]--;
						}
					}

					if (j!=0){
						assignedPattern[l]=2;
						assignedToTest[i]=2;
					}
					else{
						assignedPattern[l]=1;
						assignedToTest[i]=1;
					}
				}
			}

//			if (j==0) printf("Node l=%d, maxPatternFound final: %d\n",l, maxPatternFound);
		}
	}

	return ret;
}

void forbidPerm(graph_t* grPattern, graph_t* grToTest, char* Mp, vsize_t j, vsize_t nPattern, vsize_t nToTest, vsize_t* F, int i){
	node_list_t* listP = &(grPattern->nodes);
	node_list_t* listT = &(grToTest->nodes);
	node_t* nPj;
	node_t* nTi;
	nPj = node_list_item(listP, j);
	nTi = node_list_item(listT, i);
	vsize_t kFatherP;
	vsize_t kFatherT;
	vsize_t kChildP;
	vsize_t kChildT;
	node_t* nChildP;
	node_t* nChildT;
	int kChildj;
	int kChildi;
	vsize_t listidChildP;
	vsize_t listidChildT;
	vsize_t listidFatherP;
	vsize_t listidFatherT;
	vsize_t lastChild;
	node_t* nFatherP;
	node_t* nFatherT;

	for (kFatherP=0; kFatherP<nPj->fathers_nb; kFatherP++){
		nFatherP=nPj->fathers[kFatherP];

		if (nFatherP->list_id < j){ // then nFatherP is already associated with a father of i
			nFatherT = node_list_item(listT, F[nFatherP->list_id*2]);
			kChildj=-1;
			kChildi=-1;
			for (kChildP=0; kChildP<nFatherP->children_nb; kChildP++){
				listidChildP=nFatherP->children[kChildP]->list_id;

				if (nFatherP->children[kChildP] == nPj){
					kChildj=kChildP;
				}

				for (kChildT=0; kChildT<nFatherT->children_nb; kChildT++){
					listidChildT=nFatherT->children[kChildT]->list_id;

					if (kChildi == -1 && nFatherT->children[kChildT] == nTi){
						kChildi=kChildT;
					}

					if (kChildj == -1){
						if (kChildi != -1 && kChildT > kChildi){
							Mp[listidChildT*nPattern+listidChildP]=0;
						}
					}

					if (kChildj != -1){
						if (kChildi == -1 || kChildT < kChildi){
							Mp[listidChildT*nPattern+listidChildP]=0;
						}
					}
				}
			}
		}
		listidFatherP=nFatherP->list_id;

		for (kFatherT=0; kFatherT<nTi->fathers_nb; kFatherT++){
			nFatherT = nTi->fathers[kFatherT];
			listidFatherT=nFatherT->list_id;

			if (Mp[listidFatherT*nPattern+listidFatherP] == 1){
				lastChild=0;
				for (kChildP=0; kChildP<nFatherP->children_nb; kChildP++){
					if (kChildP < j){ // then kChildj already in an association
						nChildP=nFatherP->children[kChildP];
						listidChildP=nChildP->list_id;
						nChildT=node_list_item(listT, F[listidChildP*2]);

						// the associated in T is a child of nFatherT ?
						for (kChildT=0; kChildT<nFatherT->children_nb; kChildT++){
							if (nFatherT->children[kChildT] == nChildT){ // then yes
								// checks that the *child number* in T is greater thant the lattest
								if (kChildT >= lastChild){
									// then this is all ok for this child, next.
									lastChild=kChildT;
									break;
								}
								else{
									// problem : if nFatherP and nFatherT were associated, there would be a permutation
									// so they can't
									Mp[listidFatherT*nPattern+listidFatherP]=0;
									break;
								}
							}
						}
						if (Mp[listidFatherT*nPattern+listidFatherP]==0) break;
					}
				}
			}
		}
	}
}

int forwardChecking(graph_t* grPattern, graph_t* grToTest, char* Mp, vsize_t j, vsize_t nPattern, vsize_t nToTest, vsize_t* F, vsize_t Fmax, char* assignedPattern, char* assignedToTest, node_list_t* listP, node_list_t* listT, char optionOnlyInduced){
	vsize_t k;
	vsize_t l;
	for (k=0; k<nToTest; k++){
		for (l=0; l<nPattern; l++){ //for (l=j+1; l<nPattern; l++){
			if(assignedToTest[k]!=0 && assignedPattern[l]!=0 && Mp[k*nPattern + l]){
				node_t* nPl = node_list_item(listP, l);
				node_t* nTk = node_list_item(listT, k);
				vsize_t fm, v, w;

				fm=Fmax-2;
				v=F[fm];
				w=F[fm+1];
				node_t* nPw = node_list_item(listP, w);
				node_t* nTv = node_list_item(listT, v);

				// booleans : 1 (true), 0 (false)
				// K(bool) : 1 (known), 0 (unknown)
				char ekv=0;
				char Kekv=0;
				char evk=0;
				char Kevk=0;
				char elw=0;
//				char Kelw=0;
				char ewl=0;
//				char Kewl=0;
//				char flk=0;
//				char Kflk=0;



        if (!optionOnlyInduced){
//        Checks if : ((k, v) and (l -- k)) => (l, w),  and  ((v, k) and (l -- k)) => (w, l)
//        "Bool" cp1 = !ekv || elw;
//        "Bool" cp2 = !evk || ewl;
          char cp1=0;
          char cp2=0;
          char Cp; // Cp=cp1 || cp2
          elw=E(nPl, nPw);
          ekv=E(nTk, nTv);
          ewl=E(nPw, nPl);
          evk=E(nTv, nTk);
          cp1 = !ekv || elw;
          cp2 = !evk || ewl;
          Cp=!(cp1 && cp2);
          if (Cp){
            Mp[k*nPattern + l]=0;
          }
        }
        else{
//        Checks if : ((k, v) and (l -- k)) iff (l, w),  and  ((v, k) and (l -- k)) iff (w, l)
//        "Bool" c1 = (ekv && elw) || (!ekv && !elw);
//        "Bool" c2 = (evk && ewl) || (!evk && !ewl);

          char c1=0;
          char c2=0;
          
          char C; // C=!(c1 && c2)=!c1 || !c2
          // C=1 iff c1=0 or c2=0
          
  //				Determining c1, beginning by the second term
          // edge (l, w) in P ?
          elw=E(nPl, nPw);
  //				Kelw=1;

          if (!elw){
            // edge (k, v) in T ?
            ekv=E(nTk, nTv);
            Kekv=1;

            if (!ekv) {
              c1=1;
            }
          }

          if (!c1){ // c1 still unknown
            //elw is (always) known
            if (elw){
              if (!Kekv){
                // edge (k, v) in T ?
                ekv=E(nTk, nTv);
                Kekv=1;
              }

              if (ekv){
                c1=1;
              }
            }
          }

  //				Determining c2, beginning by the second term, only if c1 is true
          if (c1){
            // edge (w, l) in P ?
            ewl=E(nPw, nPl);
  //					Kewl=1;

            if (!ewl){
              // edge (v, k) in T ?
              evk=E(nTv, nTk);
              Kevk=1;

              if (!evk) {
                c2=1;
              }
            }

            if (!c2){ // c2 still unknown
              //ewl is (always) known
              if (ewl){
                if (!Kevk){
                  // edge (v, k) in T ?
                  evk=E(nTv, nTk);
                  Kevk=1;
                }

                if (evk){
                  c2=1;
                }
              }
            }
          }

          // C=1 iff c1=0 or c2=0
          C=!(c1&&c2);

          if (C){
            Mp[k*nPattern + l]=0;
          }
        }
			}
		}
	}

//	debugPrintM(Mp, nPattern, nToTest);

	// exists l >= j + 1 / foreach k, Mk,l = 0 ?
	if (!optionMCS){
		for (l=j+1; l<nPattern; l++){
			char eM=0; // exists l / M[k][l]=1 ?

			for (k=0; k<nToTest; k++){
				if (Mp[k*nPattern + l]==1) {
					eM=1;
					break;
				}
			}

			if (!eM){
	//			printf("0\n");
				return 0;
			}
		}
	}
	else {
		for (l=0; l<nPattern; l++){
			for (k=0; k<nToTest; k++){
				if (assignedPattern[l]!=0 && assignedToTest[k]!=0 && Mp[k*nPattern+l]==1){
					return 1;
				}
			}
		}
		return 0;
	}
//	printf("1\n");
	return 1;
}

//char Flk(vsize_t l, vsize_t k, vsize_t* F, vsize_t Fmax){
//	return 1;
//
//	vsize_t fm2, a, b;
//
//	// l <--> k ?
//	for(fm2=0; fm2<=Fmax-1; fm2+=2){
//		a=F[fm2];
//		b=F[fm2+1];
//
//		if ((a==k && b==l)){//(a==l && b==k) ||
////			printf("yop %d %d %d %d\n", a, b, l, k);
//			return 1;
//		}
//	}
//
//	return 0;
//}

char E(node_t* n1, node_t* n2){
	vsize_t t;

	// edge (n1, n2) ?
	for (t=0; t<n1->children_nb; t++){
		if (n1->children[t] == n2){
			return 1;
		}
	}

	return 0;
}

char* BFS2(graph_t* inputGraph, vsize_t vroot, vsize_t R){
	char* matrix = calloc(R*R, sizeof(char));

	//all inputgraph nodes to unexplored(0):
	node_list_t* listI = &(inputGraph->nodes);

	vsize_t i;
	node_t* nI;

	for (i=0; i<listI->count; i++){
		nI = node_list_item(listI, i);
		nI->explored=0; //unexplored
	}

	//keep track of matches inputNode <-> node (pointers) and acts as a queue
	node_t** nodeTrack=malloc(R*2*sizeof(node_t*));
	vsize_t nnT=0;

	graph_t* graph = graph_alloc(R);
	vsize_t s=0;

	nI = node_list_item(listI, vroot);
//	nI=inputGraph->root;
	nI->explored=1; // under exploration

//	printf("\nroot: %d\n", (int)nI->node_id);

	//adds the root to the graph and track
	node_t* nG=node_list_append(&graph->nodes, s);
	s++;
	// copies nI in graph, without the parent / child stuff
	nG->explored=nI->explored;
	nG->node_id=nI->node_id;
	nG->symb=nI->symb;
//	memcpy(nG, nI, sizeof(node_t));
//	MY_FREE(nG->fathers);
//	MY_FREE(nG->children);

	//adds the track
	nodeTrack[nnT]=nI;
	nodeTrack[nnT+1]=nG;
	nnT+=2;

	node_t* child;
	vsize_t k;
	vsize_t nnTmax;
	char IsChild=1;

	while(s<R && IsChild){
		IsChild=0;
		nnTmax=nnT;

		for (i=0; i<nnTmax; i+=2){
			if (s<R){
				nI = nodeTrack[i];
				if (nI->explored==1){ //under exploration : put children under exploration and itself explored
					nI->explored=2; //explored

					for (k=0; k<nI->children_nb; k++){
						child=nI->children[k];

						if (s<R && child->explored==0){
							IsChild=1;
							child->explored=1;
							//adds the child to the graph and track
							node_t* nG=node_list_append(&graph->nodes, s);
							s++;
							// copies nI in graph, without the parent / child stuff
							nG->explored=child->explored;
							nG->node_id=child->node_id;
							nG->symb=child->symb;
//							memcpy(nG, child, sizeof(node_t));
//							MY_FREE(nG->fathers);
//							MY_FREE(nG->children);

							//adds the track
							nodeTrack[nnT]=child;
							nodeTrack[nnT+1]=nG;
							nnT+=2;

//							printf("explored: %d, father: %d\n", (int) child->node_id, (int) nI->node_id);
						}
					}
				}
			}
			else{
				break;
			}
		}
	}

	if (s!=R){
		graph_free(graph);
//		printf("error\n");
		return NULL;
	}

	// now we need to :
	// -> create the edges for nodes in inputGraph and explored (or under exploration) which children also are explored (or under expl)

	if (optionCheckSymb){
		for (i=0; i<nnT; i+=2){
			nI=nodeTrack[i];
			matrix[(i/2)*R+(i/2)]=nI->symb;
		}
	}

	vsize_t j;

	for (i=0; i<nnT; i+=2){
		nI=nodeTrack[i];
		nG=nodeTrack[i+1];

//		printf("nI: %p, nG: %p\n", nI, nG);

		for (k=0; k<nI->children_nb; k++){
			child=nI->children[k];

			if (child->explored!=0){
				//find which nodeTrack it is

				for (j=0; j<nnT; j+=2){
					if (nodeTrack[j]==child){
						matrix[(i/2)*R+j/2]=1;
//						node_link(nG, nodeTrack[j+1]);
						break;
					}
				}
			}
		}
	}

//	printf("s: %d\n", s);

//	node_list_t* list = &(graph->nodes);
//	node_t* root = node_list_item(list, 0);
//	graph->root=root;

	graph_free(graph);
	free(nodeTrack);
	return matrix;
}

graph_t* BFS_gba(graph_t* inputGraph, vsize_t vroot, vsize_t R){
	//all inputgraph nodes to unexplored(0):
	node_list_t* listI = &(inputGraph->nodes);

	vsize_t i;
	node_t* nI;

	for (i=0; i<listI->count; i++){
		nI = node_list_item(listI, i);
		nI->explored=0; //unexplored
	}

	//keep track of matches inputNode <-> node (pointers) and acts as a queue
	node_t** nodeTrack=malloc(R*2*sizeof(node_t*));
	vsize_t nnT=0;

	graph_t* graph = graph_alloc(R);
	vsize_t s=0;

	nI = node_list_item(listI, vroot);
//	nI=inputGraph->root;
	nI->explored=1; // under exploration

//	printf("\nroot: %d\n", (int)nI->node_id);

	//adds the root to the graph and track
	node_t* nG=node_list_append(&graph->nodes, s);
	s++;
	// copies nI in graph, without the parent / child stuff
	nG->explored=nI->explored;
	nG->node_id=nI->node_id;
	nG->symb=nI->symb;
//	memcpy(nG, nI, sizeof(node_t));
//	MY_FREE(nG->fathers);
//	MY_FREE(nG->children);

	//adds the track
	nodeTrack[nnT]=nI;
	nodeTrack[nnT+1]=nG;
	nnT+=2;

	node_t* child;
	vsize_t k;
	vsize_t nnTmax;
	char IsChild=1;

	while(s<R && IsChild){
		IsChild=0;
		nnTmax=nnT;

		for (i=0; i<nnTmax; i+=2){
			if (s<R){
				nI = nodeTrack[i];
				if (nI->explored==1){ //under exploration : put children under exploration and itself explored
					nI->explored=2; //explored

					for (k=0; k<nI->children_nb; k++){
						child=nI->children[k];

						if (s<R && child->explored==0){
							IsChild=1;
							child->explored=1;
							//adds the child to the graph and track
							node_t* nG=node_list_append(&graph->nodes, s);
							s++;
							// copies nI in graph, without the parent / child stuff
							nG->explored=child->explored;
							nG->node_id=child->node_id;
							nG->symb=child->symb;
//							memcpy(nG, child, sizeof(node_t));
//							MY_FREE(nG->fathers);
//							MY_FREE(nG->children);

							//adds the track
							nodeTrack[nnT]=child;
							nodeTrack[nnT+1]=nG;
							nnT+=2;

//							printf("explored: %d, father: %d\n", (int) child->node_id, (int) nI->node_id);
						}
					}
				}
			}
			else{
				break;
			}
		}
	}

	// now we need to :
	// -> create the edges for nodes in inputGraph and explored (or under exploration) which children also are explored (or under expl)

	vsize_t j;

	for (i=0; i<nnT; i+=2){
		nI=nodeTrack[i];
		nG=nodeTrack[i+1];

//		printf("nI: %p, nG: %p\n", nI, nG);

		for (k=0; k<nI->children_nb; k++){
			child=nI->children[k];

			if (child->explored!=0){
				//find which nodeTrack it is

				for (j=0; j<nnT; j+=2){
					if (nodeTrack[j]==child){
						node_link(nG, nodeTrack[j+1]);
						break;
					}
				}
			}
		}
	}

//	printf("s: %d\n", s);

	node_list_t* list = &(graph->nodes);
	node_t* root = node_list_item(list, 0);
	graph->root=root;

	free(nodeTrack);

	if (s==R){
		return graph;
	}
	else{
		graph_free(graph);
		return NULL;
	}
}

int computeInterval(graph_t* inputGraph, node_t* head, node_t** intervals, int nInterval){
//	printf("c1\n");
	node_list_t* listI = &(inputGraph->nodes);
	int sizeI=listI->count;
//	printf("c2\n");

	int maxI=0;
	intervals[nInterval*sizeI+0]=head;
	maxI++;
	node_t* nI;
	int i;
	int j;
	int p;
//	printf("c3\n");
	int pOk=0;
	int added=1;
//	char alreadyIn=0;

//	printf("head : %d\n", (int) head->node_id);

	while (added==1){
		added=0;

		for (i=0; i<listI->count; i++){
			nI = node_list_item(listI, i);
//			printf("i: %d ", i);

			// is it not the root ?
			if (nI != inputGraph->root){
//				printf("noth ");
				// is it already in an interval ?
				if (isNodeIn(nI, intervals, nInterval*sizeI, maxI)){
//				if (nI->explored!=0){
//					printf("alr\n");
					continue;
				}

//				printf("noalr ");
				// do "all edges entering nI leave nodes in interval" ?
				// all parents must be in interval
				for (p=0; p<nI->fathers_nb; p++){
					pOk=0;

					// is father in interval ?
					for (j=0; j<maxI; j++){
						if (nI->fathers[p] == intervals[nInterval*sizeI+j]){
							// yes it is
//							printf("fatherIn ");
							pOk=1;
							break;
						}

//						printf("father : %d", (int) nI->fathers[p]->node_id);
					}

					// not it was not
					if (pOk==0) break;
				}

				if (pOk==0){
//					printf("noAdd\n");
					continue;
				}

				// yes it is ok :
				// add i to interval
				intervals[nInterval*sizeI+maxI]=nI;
				maxI++;
				added=1;
				nI->explored=2; //explored
//				printf("%d\n", (int) nI->node_id);
//				printf("Add\n");
			}
		}
	}

	return maxI;
}

char isNodeIn(node_t* node, node_t** intervals, int nRow, int maxI){
	int j;

	for (j=0; j<maxI; j++){
		if (node == intervals[nRow+j]){
			return 1;
		}
	}

	return 0;
}

graph_t* reduceInterval(graph_t* inputGraph){
	node_list_t* listI = &(inputGraph->nodes);
	graph_t* reducedGraph=graph_alloc(listI->count);
	node_list_t* listR = &(reducedGraph->nodes);

//	node_t*** intervals=malloc(listI->count*listI->count*sizeof(node_t*));
//	printf("s:%d, node_t*: %d, char: %d\n", listI->count, sizeof(node_t*), sizeof(char));
//	node_t* intervals[listI->count][listI->count];
		node_t** intervals=malloc(listI->count*listI->count*sizeof(node_t*)); // 2 "dimension array"
//	node_t* intervals[500][1200];
//	node_t* intervals[800][1200];

	node_t* headers[listI->count];
	int maxH=0;
	int currH=0;
	headers[0]=inputGraph->root;
	maxH++;
	int nIntervals=0;
	int maxI[listI->count];
	int i, j;
	node_t* nI;
	node_t* n;

	for (i=0; i<listI->count; i++){
		nI = node_list_item(listI, i);
		nI->explored=0; //unexplored
	}

	while(maxH > currH){
//		printf("nIntervals: %d\n", nIntervals);
		maxI[nIntervals]=computeInterval(inputGraph, headers[currH], intervals, nIntervals);
//		printf("maxI[nInt]: %d\n", maxI[nIntervals]);

		// add in H all nodes not i previous intervals (explored!) and who has a direct ancestor in last interval
		for (i=0; i<listI->count; i++){
			nI = node_list_item(listI, i);

			if (nI->explored==0 && !isNodeIn(nI, headers, 0, maxH)){
				for (j=0; j<nI->fathers_nb; j++){
					if (isNodeIn(nI->fathers[j], intervals, nIntervals*listI->count, maxI[nIntervals])){
						// add nI to headers:
						headers[maxH]=nI;
//						printf("Hadded: %d\n", (int) nI->node_id);
						maxH++;
						break;
					}
				}
			}
		}
		n=node_list_append(listR, nIntervals);
		n->node_id=nIntervals;
		n->symb=9;

		currH++;
		nIntervals++;
	}


int c, k;
node_t* n2;
	for (i=0; i<nIntervals; i++){
		for (j=0; j<maxI[i]; j++){
			for (c=0; c<intervals[i*listI->count+j]->children_nb; c++){
				for (k=0; k<nIntervals; k++){
					if (isNodeIn(intervals[i*listI->count+j]->children[c], intervals, k*listI->count, maxI[k])){
						n = node_list_item(listR, i);
						n2 = node_list_item(listR, k);
						if (!E(n, n2) && n!=n2) node_link(n, n2);
					}
				}
			}
		}
	}

	reducedGraph->root=node_list_item(listR, 0);

//	printf("nIntervals: %d\n", nIntervals);
	return reducedGraph;
}
