%{
#include <stdio.h>

extern int yylineno;

int yylex(void);
void yyerror(char const *s);

// node_t create_node(x, y)
// node_t create_node_leaf(x, value_t y)

%}

%define parse.error verbose
%define parse.trace true

%code requires {
    #include "utils.h"
    #include "arvore.h"
    extern void* arvore;
}

%union {
  node_t *nodo;
  valor_lexico *valor_lexico;
}

//%type<nodo> expr_terminais
//%type<nodo> ident_atrib
%type<nodo> tipo literal lista_ident_var ident_var lista_arranjo

%token<valor_lexico> TK_PR_INT
%token<valor_lexico> TK_PR_FLOAT
%token<valor_lexico> TK_PR_BOOL
%token<valor_lexico> TK_PR_CHAR
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
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT
%token<valor_lexico> TK_LIT_FALSE
%token<valor_lexico> TK_LIT_TRUE
%token<valor_lexico> TK_LIT_CHAR
%token<valor_lexico> TK_IDENTIFICADOR
%token TK_ERRO

%token '['
%token ']'
%token ','
%token ';'
%token ':'
%token '('
%token ')'
%token '{'
%token '}'
%token '+'
%token '-'
%token '*'
%token '/'
%token '%'
%token '^'
%token '<'
%token '>'
%token '='
%token '!'

%%

/* Programa */
programa: lista_elementos | ;

lista_elementos: lista_elementos elemento | elemento;

elemento: var_global | funcao;


/* Variável Global */
var_global: 
    tipo lista_ident_var ';'  {
        node_t* declara_var_global = create_node("CMD_DECLARA_VAR_GLOBAL");
        add_child(declara_var_global, $1);
        add_child(declara_var_global, $2);
        print_tree(declara_var_global);
    };

tipo: 
    TK_PR_INT       { $$ = create_leaf("TIPO_INT", $1); }
    | TK_PR_FLOAT   { $$ = create_leaf("TIPO_FLOAT", $1); }
    | TK_PR_BOOL    { $$ = create_leaf("TIPO_BOOL", $1); }
    | TK_PR_CHAR    { $$ = create_leaf("TIPO_CHAR", $1); };

lista_ident_var: 
    lista_ident_var ',' ident_var   { 
        node_t* lastLeaf = asList_getLeaf($1);  // mantem ordem esquerda - direita da lista
        add_child(lastLeaf, $3);
        $$ = $1;
    }
    | ident_var  { 
        $$ = $1; 
    };

ident_var: 
    TK_IDENTIFICADOR  { 
        $$ = create_leaf("ID_GLOBAL", $1); 
    }
    | TK_IDENTIFICADOR'['lista_arranjo']'       {
        node_t* id = create_leaf("ID_GLOBAL", $1);
        node_t* declara_arranjo = create_node("[]");
        add_child(declara_arranjo, id);
        add_child(declara_arranjo, $3);
        $$ = declara_arranjo;
    };

lista_arranjo: 
    lista_arranjo'^'TK_LIT_INT  {
        node_t* lastLeaf = asList_getLeaf($1);  // mantem ordem esquerda - direita da lista
        node_t* lit = create_leaf("LIT_INT", $3);
        add_child(lastLeaf, lit);
        $$ = $1;
    }
    | TK_LIT_INT { 
        $$ = create_leaf("LIT_INT", $1); 
    };


/* Função */
funcao: tipo TK_IDENTIFICADOR '(' func_params ')' command_block;

func_params: lista_params | ;

lista_params: lista_params ',' param | param;

param: tipo TK_IDENTIFICADOR;


/* Bloco de Comandos */
command_block: '{'lista_commands'}';

lista_commands: lista_commands command ';' | ;

command: command_block | declara_var | atrib | chamada_func | retorno | condicional | iteracao;

/* Declaração de Variável */

declara_var: tipo lista_local_var;

lista_local_var: lista_local_var ',' local_var | local_var;

local_var: TK_IDENTIFICADOR | TK_IDENTIFICADOR TK_OC_LE literal;

literal: 
    TK_LIT_INT      { $$ = create_leaf("LIT_INT", $1); }
    | TK_LIT_FLOAT  { $$ = create_leaf("LIT_FLOAT", $1); }
    | TK_LIT_FALSE  { $$ = create_leaf("LIT_FALSE", $1); }
    | TK_LIT_TRUE   { $$ = create_leaf("LIT_TRUE", $1); }
    | TK_LIT_CHAR   { $$ = create_leaf("LIT_CHAR", $1); };

/* Atribuição */

atrib: ident_atrib '=' expr;

ident_atrib:
    TK_IDENTIFICADOR { 
        //$$ = create_leaf("ID_GLOBAL", $1); 
    }
    | TK_IDENTIFICADOR'['lista_arranjo_atrib']' {
        //$$ = create_leaf("ID_GLOBAL", $1); 
    };

lista_arranjo_atrib: lista_arranjo_atrib'^'expr | expr;

/* Chamada de Função */

chamada_func: TK_IDENTIFICADOR'('chamada_params')';

chamada_params: chamada_lista_params | ;

chamada_lista_params: chamada_lista_params ',' expr | expr;

/* Comando de Retorno */

retorno: TK_PR_RETURN expr;

/* Condicional */

condicional: TK_PR_IF '('expr')' TK_PR_THEN command_block |
             TK_PR_IF '('expr')' TK_PR_THEN command_block TK_PR_ELSE command_block;

/* Iteração */

iteracao: TK_PR_WHILE '('expr')' command_block;

/* Expressão */

lista_expr: lista_expr '^' expr | expr;

expr: expr_preced0;

expr_preced0: expr_preced0 TK_OC_OR expr_preced1 |
              expr_preced1;

expr_preced1: expr_preced1 TK_OC_AND expr_preced2 |
              expr_preced2;

expr_preced2: expr_preced2 TK_OC_EQ expr_preced3 |
              expr_preced2 TK_OC_NE expr_preced3 |
              expr_preced3;

expr_preced3: expr_preced3 '<' expr_preced4 |
              expr_preced3 '>' expr_preced4 |
              expr_preced3 TK_OC_LE expr_preced4 |
              expr_preced3 TK_OC_GE expr_preced4 |
              expr_preced4;

expr_preced4: expr_preced4 '+' expr_preced5 |
              expr_preced4 '-' expr_preced5 |
              expr_preced5;

expr_preced5: expr_preced5 '*' expr_preced6 |
              expr_preced5 '/' expr_preced6 |
              expr_preced5 '%' expr_preced6 |
              expr_preced6;

expr_preced6: '-' expr_terminais  |
              '!' expr_terminais  |
              expr_terminais;

expr_terminais: 
    TK_IDENTIFICADOR  { 
        //$$ = create_leaf("ID_EXPR", $1); 
    }
	| TK_IDENTIFICADOR '[' lista_expr ']'  { 
        node_t* indexador = create_node("[]");
        //add_child(indexador, create_leaf("ID_EXPR", $1));
        //add_child(indexador, create_leaf("INDEX_EXPR", $2));
        //$$ = indexador; 
    }
	| literal  {
        //$$ = $1;
    }
	| '(' expr ')'  {
        //$$ = $1;
    }
	| chamada_func  {
        //$$ = $1;
    };

%%


void yyerror(char const *s) {

  printf("Erro Sintático [%s] na linha %d\n", s, yylineno);
  return; 

}


