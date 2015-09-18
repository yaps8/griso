%{
 
/*
 * Lexer.l file
 * To generate the lexical analyzer run: "flex Lexer.l"
 */
 
#include "Expression.h"
#include "Parser.h"

#include <stdio.h>
 
%}

%option outfile="Lexer.c" header-file="Lexer.h"
%option warn nodefault
 
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge
 
DIGRAPH_HEADER  [dD]igraph
ID        \"?[a-zA-Z0-9]*\"?
OPTION_STR    zzzz[a-zA-Z0-9]*
LENS      "{"
RENS      "}"
LCRO      "["
RCRO      "]"
EQ        "="
VIRG        ","
ARROW      "->"
 
WS          [ \r\n\t]*
 
%%
 
{WS}            { /* Skip blanks. */ }
 
{DIGRAPH_HEADER}      { debug_print("digraph header!\n"); return TOKEN_DIGRAPH_HEADER; }
{ID}        	{ debug_print("id: "); yylval->type_string = strdup(yytext); return TOKEN_ID; }
{OPTION_STR}    { debug_print("option_str :"); yylval->type_string = strdup(yytext); return TOKEN_ID; }
{LENS}        { debug_print("lens!\n"); return TOKEN_LENS; }
{RENS}        { debug_print("rens!\n"); return TOKEN_RENS; }
{LCRO}        { debug_print("lcro!\n"); return TOKEN_LCRO; }
{RCRO}        { debug_print("rcro!\n"); return TOKEN_RCRO; }
{EQ}        { debug_print("eq!\n"); return TOKEN_EQ; }
{VIRG}        { debug_print("virg!\n"); return TOKEN_VIRG; }
{ARROW}        { debug_print("arrow!\n"); return TOKEN_ARROW; }
.               {  }
 
%%
 
int yyerror(const char *msg) {
    fprintf(stderr,"Error!:%s\n",msg);
    fprintf(stderr, "Error while parsing dot file. Exiting.\n");
    exit(1);
    // return 0;
}