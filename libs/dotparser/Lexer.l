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
 
NUMBER      [0-9]+
WS          [ \r\n\t]*
 
%%
 
{WS}            { /* Skip blanks. */ }
{NUMBER}        { sscanf(yytext, "%d", &yylval->value); return TOKEN_NUMBER; }
 
{DIGRAPH_HEADER}      { printf("digraph header!\n"); return TOKEN_DIGRAPH_HEADER; }
{ID}        	{ printf("id: "); yylval->type_string = strdup(yytext); printf("%s\n", yylval->type_string); return TOKEN_ID; }
{OPTION_STR}    { printf("option_str :"); yylval->type_string = strdup(yytext); printf("%s\n", yylval->type_string); return TOKEN_ID; }
{LENS}        { printf("lens!\n"); return TOKEN_LENS; }
{RENS}        { printf("rens!\n"); return TOKEN_RENS; }
{LCRO}        { printf("lcro!\n"); return TOKEN_LCRO; }
{RCRO}        { printf("rcro!\n"); return TOKEN_RCRO; }
{EQ}        { printf("eq!\n"); return TOKEN_EQ; }
{VIRG}        { printf("virg!\n"); return TOKEN_VIRG; }
{ARROW}        { printf("arrow!\n"); return TOKEN_ARROW; }
.               {  }
 
%%
 
int yyerror(const char *msg) {
    fprintf(stderr,"Error!:%s\n",msg);
    fprintf(stderr, "Error while parsing dot file. Exiting.\n");
    exit(1);
    // return 0;
}