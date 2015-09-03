#include "node_list.h"

#include "node.h"
#include "rb.h"

/*!
 @brief Iterator structure to examine all nodes, in sorted by rb_tree.
 */
typedef struct node_list_iterator_t {
  /*!
   @brief  Iterator from rb_tree
   */
  struct rb_traverser rb_table_trav;
} node_list_iterator_t;

int node_rb_comp(const node_t* node1, const node_t* node2, const void *unused) {
  /* 1st sort criteria: addresses */
  if (node1->node_id > node2->node_id)
    return 1;
  if (node1->node_id < node2->node_id)
    return -1;
  return 0;
}

void node_list_build(node_list_t * list, vsize_t max_size) {
  list->size = max_size;
  list->count = 0;
  list->storage = MY_ZALLOC(max_size, node_t);
  list->rb = rb_create((rb_comparison_func*) node_rb_comp, NULL, NULL);
}

void node_list_free(node_list_t * list) {
  vsize_t i;
  for (i = 0; i < list->size; i++) {
    MY_FREE(list->storage[i].children);
    MY_FREE(list->storage[i].fathers);
  }
  MY_FREE(list->storage);
  rb_destroy(list->rb, NULL);
}

void node_list_reset(node_list_t * list) {
  vsize_t i;
  for (i = 0; i < list->count; i++) {
    node_reset(&list->storage[i]);
  }
  list->count = 0;
  rb_destroy(list->rb, NULL);
  list->rb = rb_create((rb_comparison_func*) node_rb_comp, NULL, NULL);
}

node_t * node_list_item(const node_list_t * list, vsize_t index) {
  return &list->storage[index];
}

void node_list_set_all_unexplored(node_list_t * list) {
  vsize_t i;
  for (i = 0; i < list->count; i++) {
    list->storage[i].explored = UNEXPLORED;
  }
}

vsize_t node_list_size(const node_list_t * list) {
  return list->count;
}

vsize_t node_list_size_reduc(const node_list_t * list) {
  vsize_t index,Node_count;

  Node_count=0;
  for (index = 0; index < node_list_size(list); ++index) {
	    node_t* node = node_list_item(list, index);
	    if (node->fathers_nb ==0 && node->children_nb ==0)
	    	continue;
	    Node_count++;
   }
  return Node_count;
}

node_t* node_list_append(node_list_t * list, uint64_t node_id) {
  node_t *new_node;

  MY_ASSERT_MSG(list->count < list->size, "Too small node_list");

  new_node = &list->storage[list->count];
  new_node->node_id = node_id;
  new_node->list_id = list->count;
  list->count++;

  // le noeud à ajouter est déjà dans le RB tree, mais cette erreur n'a pas d'incidence sur le résultat et le fonctionnement
  // on affiche l'erreur sans quitter
  MY_ASSERT_MSG_CONTINUE(rb_find(list->rb, &new_node) == NULL, "Already in node_list");

  rb_probe(list->rb, new_node);

  return new_node;
}

node_t* node_list_find(node_list_t * list, uint64_t node_id) {
  node_t item;
  item.node_id = node_id;
  return (node_t*) rb_find(list->rb, &item);
}

node_list_iterator_t* node_list_it_new(const node_list_t * list) {
  node_list_iterator_t *it;

  it = MY_MALLOC(1, node_list_iterator_t);
  rb_t_init(&it->rb_table_trav, list->rb);

  return it;
}

void node_list_it_free(struct node_list_iterator_t* it) {
  MY_FREE(it);
}

void node_list_it_rewind(node_list_iterator_t *it) {
  rb_t_init(&it->rb_table_trav, it->rb_table_trav.rb_table);
}

node_t * node_list_it_get_next(node_list_iterator_t *it) {
  return (node_t *) rb_t_next(&it->rb_table_trav);
}

node_t * node_list_it_get_prev(node_list_iterator_t *it) {
  return (node_t *) rb_t_prev(&it->rb_table_trav);
}
