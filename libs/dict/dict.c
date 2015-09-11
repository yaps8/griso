#include "dict.h"



int compar(const void *l, const void *r)
{
  const dictPair *dl = l;
  const dictPair *dr = r;
  
  if (dl->key < dr->key) return -1;
  if (dl->key > dr->key) return 1;
  return 0;
}

struct dict* dict_alloc(){
  struct dict* d = (struct dict*) malloc(sizeof(dict));
  d->root = NULL;
  return d;
}

node_t* dict_insert(struct dict* d, uint64_t k, node_t* v){
  dictPair* p = malloc(sizeof(dictPair));
  p->key = k;
  p->value = v;

  const void* r = tsearch(p, &d->root, compar);

  return (*(dictPair **)r)->value;
}


node_t* dict_find(struct dict* d, uint64_t k){
  dictPair* p = malloc(sizeof(dictPair));
  p->key = k;
  
  dictPair* r = (dictPair*) tfind(p, &d->root, compar);
  
  if (r == NULL) return NULL;
  else return (*(dictPair **)r)->value;
}


void dict_free(struct dict* d){
  tdestroy(d->root, free);
}