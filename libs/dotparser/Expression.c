/*
 * Expression.c
 * Implementation of functions used to build the syntax tree.
 */

#include "Expression.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Allocates space for expression
 * @return The expression or NULL if not enough memory
 */
static SExpression *allocateExpression()
{
    SExpression *b = (SExpression *)malloc(sizeof(SExpression));

    if (b == NULL)
        return NULL;

    b->type = eVALUE;
    b->value = 0;

    b->left = NULL;
    b->right = NULL;

    return b;
}

SExpression *createID(char* str_value)
{
    SExpression *b = allocateExpression();

    if (b == NULL)
        return NULL;

    b->type = eSTR;
    b->str_value = str_value;

    return b;
}

graph_t* fixDict(graph_t* g){
  vsize_t i;
  for (i=0; i<g->nodes.size; i++){
    printf("deleting and adding again...\n");
    dict_delete(g->nodes.nodes_dict, g->nodes.storage[i].node_id);
    dict_insert(g->nodes.nodes_dict, g->nodes.storage[i].node_id, &g->nodes.storage[i]);
  }
  
  return g;
}

CoupleList* createEdgeList(){
  printf("create edge list\n");
  CoupleList* cl = (CoupleList*) malloc(sizeof(CoupleList));
  cl->size = 0;
  cl->couples = NULL;
  
  return cl;
}

CoupleList* addEdgeToList(Couple* c, CoupleList* cl){
  printf("adding edge\n");
  if (cl->couples == NULL){
    cl->couples = (Couple**) malloc(sizeof(Couple*));
  }
  else {
    cl->couples = (Couple**) realloc(cl->couples, (cl->size+1) * sizeof(Couple*));
  }
  cl->couples[cl->size] = c;
  cl->size++;
  
  return cl;
}

Couple* createEdge(char* f, char* c){
  Couple* e = (Couple*) malloc(sizeof(Couple));
  e->x = hash(f);
  e->y = hash(c);
  return e;
}

graph_t* addEdgesToGraph(CoupleList* cl, graph_t* g){
  vsize_t i;
  node_t* f;
  node_t* c;
  
  for (i=0; i<cl->size; i++){
    f = dict_find(g->nodes.nodes_dict, cl->couples[i]->x);
    c = dict_find(g->nodes.nodes_dict, cl->couples[i]->y);
    
    printf("%x -> %x\n", f->node_id, c->node_id);
    
    if (f == NULL || c == NULL){
	printf("WARNING: when adding a node, father or child was not found in graph.\n");
    }
    else {
// 	int k;
// 	for (k=0; k<g->nodes.size; k++){
// 	  printf("%d: %p\n", k, &g->nodes.storage[k]);  
// 	}
      
// 	printf("linking\n");
	node_link(f, c);
// 	printf("%p (%x) != %p (%x)\n", f, f->node_id, &g->nodes.storage[0], (&g->nodes.storage[0])->node_id);
// 	node_link(&g->nodes.storage[0], c);
    }
  }
  
  graph_fprint(stdout, g);
  return g;
}

uint hash(char* s) {
    uint h = 0;
    int i;
    
    for (i = 0; i < strlen(s); i++) {
      if (s[i] != '"'){
        h = 131 * h + s[i];
      }
    }
    return h;
}

node_t *createNode(char* value){
  uint id = hash(value);
  printf("create node %s: %x\n", value, id);

  node_t* node = (node_t*) malloc(sizeof(node_t));
  node->children_nb = 0;
  node->fathers_nb = 0;
  node->explored = UNEXPLORED;
  node->list_id = 0;
  node->node_id = id;
  node->symb = INST_SEQ;

  return node;
}

graph_t *createGraph()
{
  printf("create graph\n");
//   node_t* node;
  graph_t* graph = NULL;

  graph = graph_alloc(0);

//   node = node_list_append(&graph->nodes, 0);
//   node->children_nb = 0;
//   node->fathers_nb = 0;
//   node->explored = UNEXPLORED;
//   node->list_id = 0;
//   node->node_id = 0;
//   node->symb = INST_SEQ;
//   graph->root = node;
  
  graph_fprint(stdout, graph);

  return graph;
}

graph_t* addNodeToGraph(node_t* n, graph_t* g){
  printf("adding node\n");
  node_list_add(&g->nodes, n);
  node_t* r = dict_find(g->nodes.nodes_dict, n->node_id);
  printf("added: %p %p\n", g->nodes.storage[g->nodes.size-1], r);
  
  if (g->nodes.size == 1) g->root = n;
  
  node_to_dot(n,(node_t*)&g->root->node_id, n->node_id, stdout);
  return g;
}

int strToSymb(char* s){
  if (strcmp(s, "INIT") == 0) {
      return 0;
  }
  else if (strcmp(s, "RET") == 0) {
      return 1;
  }
  else if (strcmp(s, "CALL") == 0) {
      return 2;
  }
  else if (strcmp(s, "JMP") == 0) {
      return 3;
  }
  else if (strcmp(s, "END") == 0) {
      return 4;
  }
  else if (strcmp(s, "SCALL") == 0) {
      return 5;
  }
  else if (strcmp(s, "UREAC") == 0) {
      return 6;
  }
  else if (strcmp(s, "UNDEF") == 0) {
      return 7;
  }
  else if (strcmp(s, "JCC") == 0) {
      return 8;
  }
  else if (strcmp(s, "SEQ") == 0 || strcmp(s, "INST") == 0) {
      return 9;
  }
  else if (strcmp(s, "PATH") == 0) {
      return 10;
  }
  else if (strcmp(s, "END") == 0) {
      return 11;
  }
  else{
      return 7;
  }
}

node_t* updateNode(OptionList* ol, node_t* n){
  printf("!!nid: %d\n", (int) n->node_id);
  
  int i;
  for (i=0; i<ol->size; i++){
      if (strcmp(ol->options[i]->id, "label") == 0){
	  n->symb = strToSymb(ol->options[i]->value);
      }
  }
  
  return n;
}

OptionList* createOptionList(){
  printf("create option list\n");
  OptionList* ol = (OptionList*) malloc(sizeof(OptionList));
  ol->size = 0;
  ol->options = NULL;
  
  return ol;
}

OptionList* addOptionToList(Option* o, OptionList* ol){
  printf("adding option\n");
  if (ol->options == NULL){
    ol->options = (Option**) malloc(sizeof(Option*));
  }
  else {
    ol->options = (Option**) realloc(ol->options, (ol->size+1) * sizeof(Option*));
  }
  ol->options[ol->size] = o;
  ol->size++;
  
  return ol;
}

Option* createOption(char* I, char* V){
  printf("create option\n");
  Option* o = (Option*) malloc(sizeof(Option));
  
  char* key = (char*) malloc(strlen(I) * sizeof(char));
  strcpy(key, I);
  
  char* value = (char*) malloc(strlen(V) * sizeof(char));
  strcpy(value, V);
  
  o->id = key;
  o->value = value;
}

SExpression *createNumber(int value)
{
    SExpression *b = allocateExpression();

    if (b == NULL)
        return NULL;

    b->type = eVALUE;
    b->value = value;

    return b;
}

SExpression *createOperation(EOperationType type, SExpression *left, SExpression *right)
{
    SExpression *b = allocateExpression();

    if (b == NULL)
        return NULL;

    b->type = type;
    b->left = left;
    b->right = right;

    return b;
}

void deleteExpression(SExpression *b)
{
    if (b == NULL)
        return;

    deleteExpression(b->left);
    deleteExpression(b->right);

    free(b);
}