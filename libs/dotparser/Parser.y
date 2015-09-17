%{
 
/*
 * Parser.y file
 * To generate the parser run: "bison Parser.y"
 */
 
#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"
// #include "graph2.h"

// int yyerror(graph_t **Sgraph, yyscan_t scanner, const char *msg) {
    // Add error handling routine as needed
// }
 
%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%output  "Parser.c"
%defines "Parser.h"
 
%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { graph_t **Sgraph }
%parse-param { yyscan_t scanner }

%union {
    char* type_string;
    int value;
    SExpression *expression2;
    graph_t* Sgraph;
    node_t* Snode;
    Option* Soption;
    OptionList* SoptionList;
    Couple* Sedge;
    CoupleList* SedgeList;
}
 
 
%token TOKEN_DIGRAPH_HEADER
%token <type_string> TOKEN_ID
%token <type_string> OPTION_ID
%token <type_string> TOKEN_OPTION_STR
%token TOKEN_LENS
%token TOKEN_RENS
%token TOKEN_LCRO
%token TOKEN_RCRO
%token TOKEN_EQ
%token TOKEN_VIRG
%token TOKEN_ARROW
%token <value> TOKEN_NUMBER

/* %type <graph_B> graph */
%type <Sgraph> graph
%type <Sgraph> node_list
%type <Snode> node
%type <Snode> node_id
%type <Sedge> edge
%type <SedgeList> edge_list
%type <Soption> option
%type <SoptionList> option_list

%%

input
    : graph { *Sgraph = $1; }
    ;
 
graph
    : 
    TOKEN_DIGRAPH_HEADER TOKEN_LENS node_list[G] edge_list[E] TOKEN_RENS { $$ = addEdgesToGraph($E, fixDict($G)); }
    | TOKEN_DIGRAPH_HEADER TOKEN_ID TOKEN_LENS node_list[G] edge_list[E] TOKEN_RENS { $$ = addEdgesToGraph($E, fixDict($G)); }
    ;

node_list
    :
    {$<Sgraph>$ = createGraph();}
    | node_list[G] node[nG] { printf("nnn\n"); $$ = addNodeToGraph($nG, $G); }
    | error { printf("Error parsing a node_list.\n"); }
    ;
   
node
    :
    node_id[N] TOKEN_LCRO option_list[O] TOKEN_RCRO { $$ = updateNode($O, $N); }
    ;
        
node_id
    :
    TOKEN_ID { $$ = createNode($1); }
    ;
    
option_list
    :
    {$<SoptionList>$ = createOptionList();}
    | option[O] option_list[L] {printf("listlist1\n"); $$ = addOptionToList($O, $L); }
    | option[O] TOKEN_VIRG option_list[L] {printf("listlist2\n"); $$ = addOptionToList($O, $L); }
    | error { printf("Error parsing an option_list.\n"); }
    ;
    
option
    :
    TOKEN_ID[I] TOKEN_EQ TOKEN_ID[V] { $$ = createOption($I, $V); }
    ;
    
edge_list
    :
    {$<SoptionList>$ = createEdgeList();}
    | edge[E] edge_list[L] {printf("edgeedge2\n"); $$ = addEdgeToList($E, $L); }
    | error { printf("Error parsing an edge_list.\n"); }
    ;
    
edge
    :
    TOKEN_ID[F] TOKEN_ARROW TOKEN_ID[C] TOKEN_LCRO option_list TOKEN_RCRO { $$ = createEdge($F, $C); }
    | TOKEN_ID[F] TOKEN_ARROW TOKEN_ID[C] { $$ = createEdge($F, $C); }
    ;

 
%%
