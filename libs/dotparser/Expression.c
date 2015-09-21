/*
 * Expression.c
 * Implementation of functions used to build the syntax tree.
 */

#include "Expression.h"

#include <stdlib.h>
#include <stdio.h>

void debug_print(char* s){
  if (1==0) printf("%s", s);
}

graph_t* fixDict(graph_t* g){
  vsize_t i;
  for (i=0; i<g->nodes.size; i++){
    dict_delete(g->nodes.nodes_dict, g->nodes.storage[i].node_id);
    node_t* n = dict_insert(g->nodes.nodes_dict, g->nodes.storage[i].node_id, &g->nodes.storage[i]);
  }
  
  return g;
}

CoupleList* createEdgeList(){
  CoupleList* cl = (CoupleList*) malloc(sizeof(CoupleList));
  cl->size = 0;
  cl->couples = NULL;
  
  return cl;
}

CoupleList* addEdgeToList(Couple* c, CoupleList* cl){
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
  int i;
  node_t* f;
  node_t* c;
  
  for (i=cl->size-1; i>=0; i--){
    f = dict_find(g->nodes.nodes_dict, cl->couples[i]->x);
    c = dict_find(g->nodes.nodes_dict, cl->couples[i]->y);
    
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
  
//   graph_fprint(stdout, g);
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

  node_t* node = (node_t*) malloc(sizeof(node_t));
  node->children = NULL;
  node->fathers = NULL;
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
  
//   graph_fprint(stdout, graph);

  return graph;
}

graph_t* addNodeToGraph(node_t* n, graph_t* g){
  node_list_add(&g->nodes, n);
  node_t* r = dict_find(g->nodes.nodes_dict, n->node_id);
  
  if (g->nodes.size == 1) g->root = n;
  
//   node_to_dot(n,(node_t*)&g->root->node_id, n->node_id, stdout);
  return g;
}

char* removeQuotes(char* s){
  char* s2 = malloc(strlen(s) * sizeof(char));
  int i;
  int k=0;
  
  for (i=0; i<strlen(s); i++){
    if (s[i] != '"'){
      s2[k] = s[i];
      k++;
    }
  }
  s2[k]=0;
  
  return s2;
}

int strToSymb(char* st){
  char* s = removeQuotes(st);
  int r;
  
  if (strcmp(s, "INIT") == 0) {
      r = SYMB_INIT;
  }
  else if (strcmp(s, "RET") == 0) {
      r = INST_RET;
  }
  else if (strcmp(s, "CALL") == 0) {
      r = INST_CALL;
  }
  else if (strcmp(s, "JMP") == 0) {
      r = INST_JMP;
  }
  else if (strcmp(s, "END") == 0) {
      r = INST_END;
  }
  else if (strcmp(s, "SCALL") == 0) {
      r = INST_SCALL;
  }
  else if (strcmp(s, "UREACH") == 0) {
      r = INST_UREACH;
  }
  else if (strcmp(s, "UNDEF") == 0) {
      r = INST_UNDEF;
  }
  else if (strcmp(s, "JCC") == 0) {
      r = INST_JCC;
  }
  else if (strcmp(s, "SEQ") == 0 || strcmp(s, "INST") == 0) {
      r = INST_SEQ;
  }
  else if (strcmp(s, "PATH") == 0) {
      r = SYMB_PATH;
  }
  else if (strcmp(s, "END") == 0) {
      r = INST_END;
  }
  else{
      r = -1;
  }
  free(s);
  return r;
}

node_t* updateNode(OptionList* ol, node_t* n){
  int i;
  char hasSymb = 0;
  
  for (i=0; i<ol->size; i++){
      if (hasSymb == 0 && strcmp(ol->options[i]->id, "label") == 0){
	  n->symb = strToSymb(ol->options[i]->value);
      }
      else if (strcmp(ol->options[i]->id, "symb") == 0){
	  n->symb = strToSymb(ol->options[i]->value);
	  hasSymb = 1;
      }
  }
  
  return n;
}

OptionList* createOptionList(){
  OptionList* ol = (OptionList*) malloc(sizeof(OptionList));
  ol->size = 0;
  ol->options = NULL;
  
  return ol;
}

OptionList* addOptionToList(Option* o, OptionList* ol){
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
  Option* o = (Option*) malloc(sizeof(Option));
  
  char* key = (char*) malloc(strlen(I) * sizeof(char));
  strcpy(key, I);
  
  char* value = (char*) malloc(strlen(V) * sizeof(char));
  strcpy(value, V);
  
  o->id = key;
  o->value = value;
}