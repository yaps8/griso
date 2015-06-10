#ifndef JD_NODE_H
#define JD_NODE_H

/*!
 @file node.h
 @brief Structures and routines for nodes of a graph.
 */

#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "ga_types.h"

/*!
 @brief Constants for the status of node during graph exploration.
 @see node_t
 */
enum node_explore_t {
  UNEXPLORED = 0,
  UNDER_EXPLORATION,
  EXPLORED,
  EXPLORE_OUT_OF_BOUND
};

struct node_t;
/*!
 @brief Structure describing a node in a graph.
 */
typedef struct node_t {
  /*!
   @brief Id of the node.

   For statically extraction code address is chosen (begin or maybe end of
   block).
   */
  uint64_t node_id;

  /*!
   @brief Symbol associated to the node, type is enum symb_t but explicit size
   for a more portable data structure.
   */
  symb_t symb;

  /*!
   @brief Exploration status of the node.
   */
  enum node_explore_t explored;

  /*!
   @brief Number of fathers.
   */
  vsize_t fathers_nb;

  /*!
   Number of children.
   */
  vsize_t children_nb;

  /*!
   @brief Table of pointers to the predecessors of the node.

   To retrieve the i-th father use node_father().
   @see node_father()
   */
  struct node_t** fathers;

  /*!
   @brief Table of pointers to the successors of the node.

   To retrieve the i-th child use node_child().
   @see node_child()
   */
  struct node_t** children;

  // id of the node in the nodelist
  vsize_t list_id;
} node_t;

/*!
 @brief Allocate a node.
 @return The newly malloc'ed node.
 */
node_t* node_alloc();

/*!
 @brief Reset node.
 @param node The node to reset.
 */
void node_reset(node_t* node);

/*!
 @brief Overwrite a node with an other.

 Internal structures of node1 are free'd.
 @param node1 Pointer on the node to overwrite.
 @param node2 Pointer on the node to copy.
 @return node1
 */
node_t* node_copy(node_t* node1, const node_t* node2);

/*!
 @brief Free the node.
 @param node The node to free.
 */
void node_free(node_t* node);

/*!
 @brief Reset the node to default values.
 @param node The node to reset.
 */
void node_reset(node_t* node);

/*!
 @brief Set the number of children of the node.

 A reallocation of internal structures is done if needed.
 @param node The node to set.
 @param nb The number of children
 */
void node_set_children_nb(node_t* node, vsize_t nb);

/*!
 @brief Set the number of fathers of the node.

 A reallocation of internal structures is done if needed.
 @param node The node to set.
 @param nb The number of fathers
 */
void node_set_fathers_nb(node_t* node, vsize_t nb);

/*!
 @brief Add a child to the node children list.
 @param node the node we add a child to.
 @param child the child node.
 @see node_new_child() node_child()
 */
void node_link(node_t* node, node_t* child);

/*!
 @brief Get the i-th child of a node.
 @param node The node from which to get the child.
 @param index The index of the child to get.
 @return The i-th child of a node if i is valid, otherwise it returns a NULL
 pointer.
 */
node_t* node_child(node_t* node, size_t index);

/*!
 @brief Get the i-th child of a node, for const pointers
 @param node The node from which to get the child.
 @param index The index of the child to get.
 @return The i-th child of a node if i is valid, otherwise it returns a NULL
 pointer.
 */
const node_t* node_child_const(const node_t* node, size_t index);

/*!
 @brief Get the i-th father of a node.
 @param node The node from which to get the father.
 @param index The index of the father to get.
 @return The i-th father of a node if i is valid, otherwise it returns a NULL
 pointer.
 */
node_t* node_father(node_t* node, size_t index);

/*!
 @brief Get the i-th father of a node, for const pointers.
 @param node The node from which to get the father.
 @param index The index of the father to get.
 @return The i-th father of a node if i is valid, otherwise it returns a NULL
 pointer.
 */
const node_t* node_father_const(const node_t* node, size_t index);

/*!
 @brief Initialize the exploration status of a node to unexplored.
 @param node The node.
 */
void node_init_unexplored(node_t* node);

/*!
 @brief Remove all instances of a node from the fathers node list of a node.
 @param node The node from which remove the father.
 @param to_remove The father to remove.
 */
void node_remove_father(node_t* node, node_t* to_remove);

/*!
 @brief Remove all instances of a node from the children node list of a node.
 @param node The node from which remove the child.
 @param to_remove The child to remove.
 */
void node_remove_child(node_t* node, node_t* to_remove);

#endif /* JD_NODE_H */
