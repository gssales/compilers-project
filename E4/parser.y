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
    //#include "valor_lexico.h"
    #include "arvore.h"
    #include "tabela.h"
    #include "stringstack.h"
    extern void* arvore;
    extern void* pilha_tabelas;
    extern void* pilha_str;
}

%union {
  node_t *nodo;
  valor_lexico *valor_lexico;
}

%type<nodo> programa lista_elementos elemento funcao expr_terminais
%type<nodo> command_block lista_commands command
%type<nodo> lista_local_var local_var
%type<nodo> literal ident_atrib
%type<nodo> lista_expr expr
%type<nodo> expr_preced0 expr_preced1 expr_preced2 expr_preced3 expr_preced4 expr_preced5 expr_preced6
%type<nodo> atrib lista_arranjo_atrib chamada_func chamada_params chamada_lista_params
%type<nodo> declara_var retorno condicional iteracao

%type<valor_lexico> tipo
%type<valor_lexico> lista_arranjo

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
programa: 
    escopo_global lista_elementos  {
        arvore = $2;
        //print_debug(arvore);
        print_pilha(pilha_tabelas);
    } 
    | {
        arvore = NULL;
    };

escopo_global: {
    // cria pilha e empilha a tabela de escopo global
    pilha_tabelas = create_pilha();
    push_table(pilha_tabelas, create_symbol_table());

    // cria pilha de string simples pra adicionar tipos aos id de declaracoes
    pilha_str = create_strpilha();
}

lista_elementos: 
    lista_elementos elemento  {
        if ($2 != NULL) {
            if ($1 != NULL) {
                node_t* last_function = getLastOf($1, FUNCAO);
                add_child(last_function, $2);
                $$ = $1;
            } else {
                $$ = $2;
            }
        } else {
            $$ = $1;
        }
    }
    | elemento  { $$ = $1; };

elemento: 
    var_global { $$ = NULL; } 
    | funcao { $$ = $1; };


/* Variável Global */
var_global: tipo lista_ident_var ';' {

    // desempilha pilha_str adicionando tipo a todos
    pilha_t *p = pilha_tabelas;
    tabela_t *t = p->tabelas[p->count-1];
    add_tipos_pilha_str(pilha_str, t, $1->tk_type);

};

tipo: TK_PR_INT {
        $$ = $1;
    } | TK_PR_FLOAT {
        $$ = $1;
    } | TK_PR_BOOL {
        $$ = $1;
    } | TK_PR_CHAR {
        $$ = $1;
    };

lista_ident_var: lista_ident_var ',' ident_var | ident_var;

ident_var: 
    TK_IDENTIFICADOR  { 

        // adiciona id na pilha_str para receber tipo depois
        push_strpilha(pilha_str,$1->tk_value.s);
        //printf("Push_strpilha: %s",$1->tk_value.s);
        //print_pilha_str(pilha_str);

        // adiciona simbolo na tabela de escopo global
        simbolo_t *s = create_symbol($1->line_number);
        s->natureza = SYM_VARIAVEL;
        s->valor = $1;
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[0];
        check_declared($1->line_number, p, $1->tk_value.s);
        insert_symbol(t, $1->tk_value.s, s);

        //destroy_lexvalue($1); 
    } 
    | TK_IDENTIFICADOR'['lista_arranjo']'  { 
        // adiciona id na pilha_str para receber tipo depois
        push_strpilha(pilha_str,$1->tk_value.s);
        //printf("Push_strpilha: %s",$1->tk_value.s);
        //print_pilha_str(pilha_str);

        // adiciona arranjo na tabela de escopo global
        // FALTA INFORMACAO DE DIMENSOES DO ARRANJO
        simbolo_t *s = create_symbol($1->line_number);
        s->natureza = SYM_ARRANJO;
        s->valor = $1;
        s->tamanhoB = $3->tk_value.i; // dimensao total (multiplicada dependendo do tipo na add_tipos_pilha_str)
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[0];
        check_declared($1->line_number, p, $1->tk_value.s);
        insert_symbol(t,$1->tk_value.s, s);

        //destroy_lexvalue($1); 
    };

lista_arranjo: 
    lista_arranjo'^'TK_LIT_INT { 
        int dim_total = $1->tk_value.i + $3->tk_value.i;
        
        $1->tk_value.i = dim_total;
        $$ = $1;
        //destroy_lexvalue($3); 
    }
    | TK_LIT_INT { 
        $$ = $1;
        //destroy_lexvalue($1); 
    };


/* Função */
funcao:
    tipo TK_IDENTIFICADOR '(' func_params ')' command_block  {
        
        // add funcao na tabela de escopo atual
        // FALTA INFORMACAO DE ARGUMENTOS
        simbolo_t *s = create_symbol($2->line_number);
        s->natureza = SYM_FUNCAO;
        s->tipo = tktype_to_type($1->tk_type);
        s->valor = $2;
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[p->count-1];
        check_declared($2->line_number, p, $2->tk_value.s);
        insert_symbol(t,$2->tk_value.s, s);
        
        node_t* funcao = create_node($2->tk_value.s);
        funcao->flag = FUNCAO;
        if ($6 != NULL)
            add_child(funcao, $6);
        $$ = funcao;
        //destroy_lexvalue($2);
    };

empilha_escopo: { 
        // cria nova tabela de simbolos e empilha na pilha de escopos
        push_table(pilha_tabelas, create_symbol_table());
    };

func_params: lista_params | ;

lista_params: lista_params ',' param | param;

param: tipo TK_IDENTIFICADOR  { destroy_lexvalue($2); };


/* Bloco de Comandos */
command_block: 
    '{' empilha_escopo lista_commands'}'  { 
        $$ = $3;
        //print_tree($$);
        pop_table(pilha_tabelas);
    };

lista_commands:
    lista_commands command ';'  { 
        if ($2 != NULL) {
            //$2 é um comando
            if ($1 != NULL) {
                node_t* last_cmd = getLastOf($1, COMANDO);
                add_child(last_cmd, $2);
                $$ = $1;
            } else {
                $$ = $2;
            }
        } else {
            $$ = $1;
        }
    }
    | { $$ = NULL; };

command:  
    command_block  { $$ = $1; }
    | declara_var  { 
	if ($1 != NULL)
	        $1->flag = COMANDO;
	$$ = $1;
    } 
    | atrib {   
        $1->flag = COMANDO;
        $$ = $1;
    } 
    | chamada_func  { 
        $1->flag = COMANDO;
        $$ = $1;
    } 
    | retorno  { 
        $1->flag = COMANDO;
        $$ = $1;
    } 
    | condicional  { 
        $1->flag = COMANDO;
        $$ = $1;
    } 
    | iteracao { 
        $1->flag = COMANDO;
        $$ = $1;
    }; 

/* Declaração de Variável */

declara_var: 
    tipo lista_local_var  {

        // desempilha pilha_str adicionando tipo a todos
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[p->count-1];
        add_tipos_pilha_str(pilha_str, t, $1->tk_type);

        $$ = $2;
    };

lista_local_var:
    lista_local_var ',' local_var  {
        if ($3 != NULL) {
            if ($1 != NULL) {
                node_t* last_cmd = getLastOf($1, COMANDO);
                add_child(last_cmd, $3);
                $$ = $1;
            } else {
                $$ = $3;
            }
        } else {
            $$ = $1;
        }
    } 
    | local_var {
        if ($1 != NULL) {
            $$ = $1;
        } else {
            $$ = NULL;
        }
    };

local_var: 
    TK_IDENTIFICADOR  {
        $$ = NULL;

        // adiciona id na pilha_str para receber tipo depois
        push_strpilha(pilha_str,$1->tk_value.s);
        //printf("Push_strpilha: %s",$1->tk_value.s);
        //print_pilha_str(pilha_str);

        // adiciona var na tabela de escopo atual
        simbolo_t *s = create_symbol($1->line_number);
        s->natureza = SYM_VARIAVEL;
        s->valor = $1;
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[p->count-1];
        check_declared($1->line_number, p, $1->tk_value.s);
        insert_symbol(t,$1->tk_value.s, s);

        //destroy_lexvalue($1);
    } | 
    TK_IDENTIFICADOR TK_OC_LE literal {

        // adiciona id na pilha_str para receber tipo depois
        push_strpilha(pilha_str,$1->tk_value.s);
        //printf("Push_strpilha: %s",$1->tk_value.s);
        //print_pilha_str(pilha_str);

        // adiciona var na tabela de escopo atual
        simbolo_t *s = create_symbol($1->line_number);
        s->natureza = SYM_VARIAVEL;
        s->valor = $1;
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[p->count-1];
        check_declared($1->line_number, p, $1->tk_value.s);
        insert_symbol(t,$1->tk_value.s, s);

        node_t* inicializa = create_node("<=");
        inicializa->flag = COMANDO;
        add_child(inicializa, create_leaf($1->tk_value.s, $1));
        add_child(inicializa, $3);
        $$ = inicializa;
    };

literal: 
    TK_LIT_INT  { 
        // adiciona literal na tabela de escopo atual
        simbolo_t *s = create_symbol($1->line_number);
        s->natureza = SYM_LITERAL;
        s->tipo = TYPE_INT;
        s->valor = $1;
        s->tamanhoB = 4;
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[p->count-1];
        insert_symbol(t,$1->str, s);

        $$ = create_leaf($1->str, $1); 
    }
    | TK_LIT_FLOAT  {
        // adiciona literal na tabela de escopo atual
        simbolo_t *s = create_symbol($1->line_number);
        s->natureza = SYM_LITERAL;
        s->tipo = TYPE_FLOAT;
        s->valor = $1;
        s->tamanhoB = 8;
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[p->count-1];
        insert_symbol(t,$1->str, s);

        $$ = create_leaf($1->str, $1); 
    }
    | TK_LIT_FALSE  {
        // adiciona literal na tabela de escopo atual
        simbolo_t *s = create_symbol($1->line_number);
        s->natureza = SYM_LITERAL;
        s->tipo = TYPE_BOOL;
        s->valor = $1;
        s->tamanhoB = 1;
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[p->count-1];
        insert_symbol(t,"false", s);
        $$ = create_leaf($1->str, $1); 
    }
    | TK_LIT_TRUE   {
        // adiciona literal na tabela de escopo atual
        simbolo_t *s = create_symbol($1->line_number);
        s->natureza = SYM_LITERAL;
        s->tipo = TYPE_BOOL;
        s->valor = $1;
        s->tamanhoB = 1;
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[p->count-1];
        insert_symbol(t,"true", s);

        $$ = create_leaf($1->str, $1); 
    }
    | TK_LIT_CHAR  { 
        // adiciona literal na tabela de escopo atual
        simbolo_t *s = create_symbol($1->line_number);
        s->natureza = SYM_LITERAL;
        s->tipo = TYPE_CHAR;
        s->valor = $1;
        s->tamanhoB = 1;
        pilha_t *p = pilha_tabelas;
        tabela_t *t = p->tabelas[p->count-1];
        insert_symbol(t,$1->str, s);

        $$ = create_leaf(&$1->tk_value.c, $1); 
    };

/* Atribuição */

atrib: 
    ident_atrib '=' expr {
        node_t* atribuicao = create_node("=");
        add_child(atribuicao, $1);
        add_child(atribuicao, $3);
        $$ = atribuicao;
    };

ident_atrib:
    TK_IDENTIFICADOR  {
        // consulta a tabela pra saber se id->natureza == SYM_VARIAVEL
        pilha_t *p = pilha_tabelas;
        simbolo_t* s = get_symbol_pilha($1->line_number, p, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_VARIAVEL);

        $$ = create_leaf($1->tk_value.s, $1);
    } 
    | TK_IDENTIFICADOR'['lista_arranjo_atrib']'  {
        // consulta a tabela pra saber se id->natureza == SYM_ARRANJO
        pilha_t *p = pilha_tabelas;
        simbolo_t* s = get_symbol_pilha($1->line_number, p, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_ARRANJO);

        node_t* id_lista_atrib = create_leaf($1->tk_value.s, $1);
        node_t* indexador = create_node("[]");
        add_child(indexador, id_lista_atrib);
        add_child(indexador, $3);
        $$ = indexador;
    };

lista_arranjo_atrib:
    lista_arranjo_atrib'^'expr  {
        if ($1 != NULL) {
            node_t* lista = create_node("^");
            add_child(lista, $1);
            add_child(lista, $3);
            $$ = lista;
        }
        else {
            $$ = $3;
        }
        
    }
    | expr  { 
        //node_t* lista = create_node("^");
        //add_child(lista, $1);
        //$$ = lista;
        $$ = $1;
    };

/* Chamada de Função */

chamada_func:
    TK_IDENTIFICADOR'('chamada_params')'  {

        // consulta a tabela pra saber se id->natureza == SYM_FUNCAO
        pilha_t *p = pilha_tabelas;
        simbolo_t* s = get_symbol_pilha($1->line_number, p, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_FUNCAO);

        node_t* funcao = create_node("call ");
        strcat(funcao->label, $1->tk_value.s);
        if ($3 != NULL)
            add_child(funcao, $3);
        $$ = funcao;
        destroy_lexvalue($1);
    };

chamada_params: 
    chamada_lista_params  { $$ = $1; } 
    | { $$ = NULL; };

chamada_lista_params: 
    chamada_lista_params ',' expr  {
        node_t* last_expr = getLastOf($1, EXPRESSAO);
        add_child(last_expr, $3);
        $$ = $1;
    } | 
    expr  {
        $$ = $1;
    };

/* Comando de Retorno */

retorno:  
    TK_PR_RETURN expr  {
        node_t* cmd_ret = create_node("return");
        add_child(cmd_ret, $2);
        $$ = cmd_ret;
    };

/* Condicional */

condicional:
    TK_PR_IF '('expr')' TK_PR_THEN command_block  {
        node_t* cmd_if = create_node("if");
        add_child(cmd_if, $3);
        add_child(cmd_if, $6);
        $$ = cmd_if;
    } 
    | TK_PR_IF '('expr')' TK_PR_THEN command_block TK_PR_ELSE command_block  {
        node_t* cmd_if = create_node("if");
        add_child(cmd_if, $3);
        add_child(cmd_if, $6);
        add_child(cmd_if, $8);
        $$ = cmd_if;
    };

/* Iteração */

iteracao: 
    TK_PR_WHILE '('expr')' command_block  {
        node_t* cmd_while = create_node("while");
        add_child(cmd_while, $3);
        add_child(cmd_while, $5);
        $$ = cmd_while;
    };

/* Expressão */

lista_expr:
    lista_expr '^' expr  {
        node_t* lista = create_node("^");
        add_child(lista, $1);
        add_child(lista, $3);
        $$ = lista;
    } 
    | expr  { 
        //node_t* expr = create_node("^");
        //add_child(expr, $1);
        //$$ = expr;
        $$ = $1;
    };

expr: 
    expr_preced0  {
        $$ = $1;
        $$->flag = EXPRESSAO;
    };

expr_preced0: 
    expr_preced0 TK_OC_OR expr_preced1  { 
        $$ = create_node("||");
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced1  { $$ = $1; };

expr_preced1: 
    expr_preced1 TK_OC_AND expr_preced2  { 
        $$ = create_node("&&");
        add_child($$, $1);
        add_child($$, $3);
    } 
    | expr_preced2  { $$ = $1; };

expr_preced2:
    expr_preced2 TK_OC_EQ expr_preced3  { 
        $$ = create_node("==");
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced2 TK_OC_NE expr_preced3  { 
        $$ = create_node("!=");
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced3  { $$ = $1; };

expr_preced3:
    expr_preced3 '<' expr_preced4  { 
        $$ = create_node("<");
        add_child($$, $1);
        add_child($$, $3);
    } 
    | expr_preced3 '>' expr_preced4  { 
        $$ = create_node(">");
        add_child($$, $1);
        add_child($$, $3);
    } 
    | expr_preced3 TK_OC_LE expr_preced4  { 
        $$ = create_node("<=");
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced3 TK_OC_GE expr_preced4  { 
        $$ = create_node(">=");
        add_child($$, $1);
        add_child($$, $3);
    } 
    | expr_preced4  { $$ = $1; };            

expr_preced4: 
    expr_preced4 '+' expr_preced5  { 
        $$ = create_node("+");
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced4 '-' expr_preced5  { 
        $$ = create_node("-");
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced5  { $$ = $1; };

expr_preced5:
    expr_preced5 '*' expr_preced6  { 
        $$ = create_node("*");
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced5 '/' expr_preced6  { 
        $$ = create_node("/");
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced5 '%' expr_preced6  { 
        $$ = create_node("%");
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced6  { $$ = $1; };

expr_preced6: 
    '-' expr_terminais  { 
        $$ = create_node("-");
        add_child($$, $2);
    }
    | '!' expr_terminais  { 
        $$ = create_node("!");
        add_child($$, $2);
    }
    | expr_terminais { $$ = $1; };

expr_terminais: 
    TK_IDENTIFICADOR  { 

        // consulta a tabela pra saber se id->natureza == SYM_VARIAVEL
        pilha_t *p = pilha_tabelas;
        simbolo_t* s = get_symbol_pilha($1->line_number, p, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_VARIAVEL);

        $$ = create_leaf($1->tk_value.s, $1);
    }
    | TK_IDENTIFICADOR '[' lista_expr ']'  {

        // consulta a tabela pra saber se id->natureza == SYM_ARRANJO
        pilha_t *p = pilha_tabelas;
        simbolo_t* s = get_symbol_pilha($1->line_number, p, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_ARRANJO);

        node_t* id_lista_expr = create_leaf($1->tk_value.s, $1);
        node_t* indexador = create_node("[]");
        add_child(indexador, id_lista_expr);
        add_child(indexador, $3);
        $$ = indexador;
    }
    | literal  { $$ = $1; }
    | '(' expr ')'  { $$ = $2; }
    | chamada_func  { $$ = $1; };

%%


void yyerror(char const *s) {

  printf("Erro Sintático [%s] na linha %d\n", s, yylineno);
  return; 


}



