#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "node.h"

typedef struct
{
  u_int64_t key;
  node_t* value;
} dictPair;

struct dict{
  void* root;
} dict;

struct dict* dict_alloc();
node_t* dict_insert(struct dict* d, uint64_t k, node_t* v);
node_t* dict_find(struct dict* d, uint64_t k);
void dict_free(struct dict* d);