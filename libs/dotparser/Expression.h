

/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "graph.h"
#include "graphIO.h"
// #include <cstddef>
#include <stddef.h>
#include <string.h>

// graph_t* graph_alloc(vsize_t max_size);
// void graph_free(graph_t* graph);

/**
 * @brief The operation type
 */
typedef enum tagEOperationType
{
    eVALUE,
    eMULTIPLY,
    ePLUS,
    eSTR
} EOperationType;

/**
 * @brief The expression structure
 */
typedef struct tagSExpression
{
    EOperationType type;///< type of operation

    int value;///< valid only when type is eVALUE
    char* str_value;
    struct tagSExpression *left; ///< left side of the tree
    struct tagSExpression *right;///< right side of the tree
} SExpression;

typedef struct Couple
{
  uint x;
  uint y;
} Couple;

typedef struct CoupleList
{
  vsize_t size;
  Couple** couples;
} CoupleList;

uint hash(char* s);

CoupleList* createEdgeList();

CoupleList* addEdgeToList(Couple* c, CoupleList* cl);

Couple* createEdge(char* f, char* c);

graph_t* addEdgesToGraph(CoupleList* cl, graph_t* g);

typedef struct Option
{
  char* id;
  char* value;
} Option;

typedef struct OptionList
{
  vsize_t size;
  Option** options;
} OptionList;

node_t* updateNode(OptionList* ol, node_t* n);

OptionList* createOptionList();

OptionList* addOptionToList(Option* o, OptionList* ol);

Option* createOption(char* I, char* V);

/**
 * @brief Options
 */
/*
enum option_type
{
    OPTION_UNKNOWN,
    OPTION_LABEL,
    OPTION_ROOT,
    OPTION_FILLCOLOR,
    OPTION_TEST
};*/

/**
 * @brief It creates an identifier
 * @param value The number value
 * @return The expression or NULL in case of no memory
 */
SExpression *createNumber(int value);

/**
 * @brief It creates an ID
 * @param value The ID value
 * @return The expression or NULL in case of no memory
 */
SExpression *createID(char* value);

/**
 * @brief It creates a node
 * @param value The name of the node
 * @return The graph or NULL in case of no memory
 */
node_t *createNode(char* value);

graph_t* fixDict(graph_t* g);

graph_t *createGraph();

graph_t* addNodeToGraph(node_t* n, graph_t* g);

/**
 * @brief It creates an operation
 * @param type The operation type
 * @param left The left operand
 * @param right The right operand
 * @return The expression or NULL in case of no memory
 */
SExpression *createOperation(EOperationType type, SExpression *left, SExpression *right);

/**
 * @brief Deletes a expression
 * @param b The expression
 */
void deleteExpression(SExpression *b);

#endif // __EXPRESSION_H__
