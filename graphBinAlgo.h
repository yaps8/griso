#ifndef GRAPHBINALGO_H
#define GRAPHBINALGO_H

int main(int, char**);
int pcharcmp(char* c1, char* c2, int size);
void* processSubGraphs(void* grData);
void createGraphs(void);
int isoUllman(graph_t*, graph_t*);
void debugPrintF(vsize_t*, vsize_t);
void debugPrintM(char*, vsize_t, vsize_t);
int backtrack(graph_t* grPattern, graph_t* grToTest, char* M0, vsize_t j, vsize_t nPattern, vsize_t nToTest, vsize_t limit, vsize_t* F, vsize_t Fmax, char* assignedPattern, char* assignedToTest, vsize_t lastAssignedPatternPlusOne, node_list_t* listP, node_list_t* listT, char only_induced);
void forbidPerm(graph_t*, graph_t*, char*, vsize_t, vsize_t, vsize_t, vsize_t*, int);
int forwardChecking(graph_t* grPattern, graph_t* grToTest, char* Mp, vsize_t j, vsize_t nPattern, vsize_t nToTest, vsize_t* F, vsize_t Fmax, char* assignedPattern, char* assignedToTest, node_list_t* listP, node_list_t* listT, char only_induced);
//char Flk(vsize_t, vsize_t, vsize_t*, vsize_t);
char E(node_t*, node_t*);
char Elw(node_t*, node_t*);
char Ekv(node_t*, node_t*);
char Ewl(node_t*, node_t*);
char Evk(node_t*, node_t*);
graph_t* BFS(graph_t* inputGraph, vsize_t vroot, vsize_t R);
char* BFS2(graph_t* inputGraph, vsize_t vroot, vsize_t R);
int computeInterval(graph_t* inputGraph, node_t* head, node_t** interval, int nInterval);
char isNodeIn(node_t* node, node_t** interval, int nRow, int maxI);
graph_t* reduceInterval(graph_t* graph);

#endif
