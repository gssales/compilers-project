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

%type<nodo> expr_terminais
%type<nodo> ident_atrib
%type<nodo> tipo literal lista_ident_var ident_var lista_arranjo
%type<nodo> lista_expr expr
%type<nodo> expr_preced0 expr_preced1 expr_preced2 expr_preced3 expr_preced4 expr_preced5 expr_preced6
%type<nodo> atrib lista_arranjo_atrib chamada_func chamada_params chamada_lista_params
%type<nodo> command lista_commands command_block declara_var retorno condicional iteracao

%token<valor_lexico> TK_PR_INT
%token<valor_lexico> TK_PR_FLOAT
%token<valor_lexico> TK_PR_BOOL
%token<valor_lexico> TK_PR_CHAR
%token<valor_lexico> TK_PR_IF
%token<valor_lexico> TK_PR_THEN
%token<valor_lexico> TK_PR_ELSE
%token<valor_lexico> TK_PR_WHILE
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token<valor_lexico> TK_PR_RETURN
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
        //print_tree(declara_var_global);
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
command_block: '{'lista_commands'}' { 
                  $$ = $2;
                };

lista_commands: lista_commands command ';'  { 
                  //node_t* lastLeaf = asList_getLeaf($1);
                  //add_child(lastLeaf, $2);
                  //$$ = $1;
                  //print_tree($$);
                } | 
                { 
                  $$ = NULL; 
                };

command:  command_block { 
            //$$ = $1; 
          } |
          declara_var  { 
            //$$ = $1;
          } | 
          atrib { 
            //$$ = $1;  // FUNCIONANDO OK
          } | 
          chamada_func  { 
            //$$ = $1;
          } | 
          retorno  { 
            //$$ = $1; // FUNCIONANDO OK
          } | 
          condicional  { 
            //$$ = $1; // TESTAR
          } | 
          iteracao { 
            //$$ = $1; // TESTAR
          }; 

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

atrib: ident_atrib '=' expr {
  node_t* atribuicao = create_node("ATRIBUICAO");
        add_child(atribuicao, $1);
        add_child(atribuicao, $3);
        print_tree(atribuicao);
};

ident_atrib:
    TK_IDENTIFICADOR {
      $$ = create_leaf("ID_ATRIB", $1);
    } |
    TK_IDENTIFICADOR'['lista_arranjo_atrib']' {
      node_t* id_lista_atrib = create_leaf("ID_LISTA_ATRIB", $1);
      node_t* indexador = create_node("[]");
      add_child(indexador, id_lista_atrib);
      add_child(indexador, $3);
      $$ = indexador;
    };

lista_arranjo_atrib:  lista_arranjo_atrib'^'expr {
                      node_t* lista = create_node("^");
                      add_child(lista, $1);
                      add_child(lista, $3);
                      $$ = lista;
                  } |
                  expr  { 
                    $$ = $1;
                  };

/* Chamada de Função */

chamada_func: TK_IDENTIFICADOR'('chamada_params')'  {
                    node_t* funcao = create_node("()");
                    add_child(funcao, create_leaf("ID_FUNCAO", $1));
                    if (funcao != NULL)
                        add_child(funcao, $3); // MUDAR PRA PRIMEIRO PARAMETRO DA LISTA EM $3?
                    $$ = funcao;
                    //print_tree($$);
                };

chamada_params: 
    chamada_lista_params  {
        $$ = $1;
    } | 
    {
        $$ = NULL;
    };

chamada_lista_params: 
    chamada_lista_params ',' expr  {
        node_t* lista = create_node(",");
        add_child(lista, $1);
        add_child(lista, $3);
        $$ = lista;
    } | 
    expr  {
        $$ = $1;
    };

/* Comando de Retorno */

retorno:  TK_PR_RETURN expr {
            //node_t* cmd_ret = create_node("CMD_RETURN");
            //add_child(cmd_ret, $2);
            //$$ = cmd_ret;
            //print_tree($$); // RETORNO OK!
          };

/* Condicional */

condicional:  TK_PR_IF '('expr')' TK_PR_THEN command_block {
                //node_t* cmd_if = create_node("CMD_IF");
                //pri_comando = ??? // PEGAR PRIMEIRO COMANDO DO BLOCO EM $6
                //add_child(cmd_if, $3); // expr
                //add_child(cmd_if, pri_comando);
                //$$ = cmd_if;
              } |
             TK_PR_IF '('expr')' TK_PR_THEN command_block TK_PR_ELSE command_block {
                //node_t* cmd_if = create_node("CMD_IF");
                //pri_comando_then = ??? // PEGAR PRIMEIRO COMANDO DO BLOCO EM $6
                //pri_comando_else = ??? // PEGAR PRIMEIRO COMANDO DO BLOCO EM $8
                //add_child(cmd_if, $3); // expr
                //add_child(cmd_if, pri_comando_then);
                //add_child(cmd_if, pri_comando_else);
                //$$ = cmd_if;
             };

/* Iteração */

iteracao: TK_PR_WHILE '('expr')' command_block {
            //node_t* cmd_while = create_node("CMD_WHILE");
            //pri_comando_then = ??? // PEGAR PRIMEIRO COMANDO DO BLOCO EM $5
            //add_child(cmd_while, $3);
            //add_child(cmd_while, pri_comando_then); // PRIMEIRO COMANDO DO BLOCO?
            //$$ = cmd_while;
          };

/* Expressão */

lista_expr: lista_expr '^' expr {
                node_t* lista = create_node("^");
                add_child(lista, $1);
                add_child(lista, $3);
                $$ = lista;
              } |
              expr  { 
                $$ = $1;
              } ;

expr: expr_preced0  {
            $$ = $1; 
          };

expr_preced0: expr_preced0 TK_OC_OR expr_preced1 { 
                $$ = create_node("||");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced1  { 
                $$ = $1; 
              };

expr_preced1: expr_preced1 TK_OC_AND expr_preced2 { 
                $$ = create_node("&&");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced2  { 
                $$ = $1; 
              };

expr_preced2: expr_preced2 TK_OC_EQ expr_preced3 { 
                $$ = create_node("==");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced2 TK_OC_NE expr_preced3 { 
                $$ = create_node("!=");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced3  { 
                $$ = $1; 
              };

expr_preced3: expr_preced3 '<' expr_preced4 { 
                $$ = create_node("<");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced3 '>' expr_preced4 { 
                $$ = create_node(">");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced3 TK_OC_LE expr_preced4 { 
                $$ = create_node("<=");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced3 TK_OC_GE expr_preced4 { 
                $$ = create_node(">=");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced4  { 
                $$ = $1; 
              };

expr_preced4: expr_preced4 '+' expr_preced5 { 
                $$ = create_node("+");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced4 '-' expr_preced5 { 
                $$ = create_node("-");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced5  { 
                $$ = $1; 
              };

expr_preced5: expr_preced5 '*' expr_preced6 { 
                $$ = create_node("*");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced5 '/' expr_preced6 { 
                $$ = create_node("/");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced5 '%' expr_preced6 { 
                $$ = create_node("%");
                add_child($$, $1);
                add_child($$, $3);
              } |
              expr_preced6  { 
                $$ = $1; 
              };

expr_preced6: '-' expr_terminais  { 
                $$ = create_node("INV");
                add_child($$, $2);
              } |
              '!' expr_terminais  { 
                $$ = create_node("NEG");
                add_child($$, $2);
              } |
              expr_terminais { 
                $$ = $1; 
              };

expr_terminais: 
    TK_IDENTIFICADOR  { 
        $$ = create_leaf("ID_EXPR", $1);
    } | 
    TK_IDENTIFICADOR '[' lista_expr ']'  {
        node_t* id_lista_expr = create_leaf("ID_LISTA_EXPR", $1);
        node_t* indexador = create_node("[]");
        add_child(indexador, id_lista_expr);
        add_child(indexador, $3);
        $$ = indexador;
    } |
    literal  {
        $$ = $1;
    } |
    '(' expr ')'  {
        $$ = $2;
    }	|
    chamada_func  {
        //$$ = $1;
    };

%%


void yyerror(char const *s) {

  printf("Erro Sintático [%s] na linha %d\n", s, yylineno);
  return; 

}


