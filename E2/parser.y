%{
#include <stdio.h>

extern int yylineno;

int yylex(void);
void yyerror(char const *s);

%}

%define parse.error verbose

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_FOR
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_IDENTIFICADOR
%token TK_ERRO

%%

/* Programa */
programa: lista_elementos | ;

lista_elementos: lista_elementos elemento | elemento;

elemento: var_global | funcao;


/* Variável Global */
var_global: tipo lista_ident_var ';';

tipo: TK_PR_INT |  TK_PR_FLOAT | TK_PR_BOOL | TK_PR_CHAR;

lista_ident_var: lista_ident_var ',' ident_var | ident_var;

ident_var: ident | ident'['lista_arranjo']';

ident: TK_IDENTIFICADOR;

lista_arranjo: lista_arranjo'^'TK_LIT_INT | TK_LIT_INT;


/* Função */
funcao: tipo TK_IDENTIFICADOR '(' func_params ')' command_block;

func_params: lista_params | ;

lista_params: lista_params ',' param | param;

param: tipo TK_IDENTIFICADOR;


/* Bloco de Comandos */
command_block: '{'lista_commands'}';

lista_commands: lista_commands command | ;

command: command_block;

%%


void yyerror(char const *s) {

  printf("Erro Sintático [%s] na linha %d\n", s, yylineno);
  return; 

}


