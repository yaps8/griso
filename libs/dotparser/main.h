#ifndef DICT_H
#define DICT_H


// int yyparse(graph_t **graph, yyscan_t scanner);

/*
 * main.c file
 */

#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"
#include "graph.h"
#include "graphIO.h"
 
#include <stdio.h>
 
 
graph_t *getGraph(const char *expr); 
int main(void);

#endif