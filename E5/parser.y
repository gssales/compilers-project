%{
#include <stdio.h>

extern int yylineno;

int yylex(void);
void yyerror(char const *s);

%}

%define parse.error verbose
%define parse.trace true

%code requires {
    #include "lexvalue.h"
    #include "arvore.h"
    #include "tabela.h"
    #include "inference.h"
    #include "stringstack.h"
    extern void* arvore;
    //stack_t* table_stack;
    extern void* table_stack;
    //strstack_t* strstack;
    extern void* strstack;
    extern int rfp;
    extern int rbss;
}

%union {
  node_t *nodo;
  lexvalue_t *lexvalue;
}

%type<nodo> programa lista_elementos elemento funcao expr_terminais
%type<nodo> command_block command_block_no_new_scope lista_commands command
%type<nodo> lista_local_var local_var
%type<nodo> literal ident_atrib
%type<nodo> lista_expr expr
%type<nodo> expr_preced0 expr_preced1 expr_preced2 expr_preced3 expr_preced4 expr_preced5 expr_preced6
%type<nodo> atrib lista_arranjo_atrib chamada_func chamada_params chamada_lista_params
%type<nodo> declara_var retorno condicional iteracao

%type<lexvalue> tipo
%type<lexvalue> lista_arranjo

%token<lexvalue> TK_PR_INT
%token<lexvalue> TK_PR_FLOAT
%token<lexvalue> TK_PR_BOOL
%token<lexvalue> TK_PR_CHAR
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
%token<lexvalue> TK_LIT_INT
%token<lexvalue> TK_LIT_FLOAT
%token<lexvalue> TK_LIT_FALSE
%token<lexvalue> TK_LIT_TRUE
%token<lexvalue> TK_LIT_CHAR
%token<lexvalue> TK_IDENTIFICADOR
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

        table_t *to = pop_table(table_stack);
        //print_table(to);

        destroy_stack(table_stack);
        destroy_strstack(strstack);

        print_program($2->code);
        destroy_iloc_program($2->code);

    } 
    | {
        arvore = NULL;
    };

escopo_global: {
    // cria pilha e empilha a tabela de escopo global
    table_stack = create_stack();
    push_table(table_stack, create_symbol_table(1));

    // cria pilha de string simples pra adicionar tipos aos id de declaracoes
    strstack = create_strstack();
}

lista_elementos: 
    lista_elementos elemento  {
        if ($2 != NULL) {
            if ($1 != NULL) {
                node_t* last_function = get_last_of($1, AST_FUNCTION);
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

    // desempilha strstack adicionando tipo a todos
    stack_t* ts = table_stack;
    table_t *t = get_table(ts, ts->count-1);
    add_types_to_strstack(strstack, t, $1->tk_type, 1);

    destroy_lexvalue($1);
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

        // adiciona id na strstack para receber tipo depois
        push_strstack(strstack,$1->tk_value.s);

        // adiciona simbolo na tabela de escopo global
        symbol_t *s = create_symbol_id($1->line_number, $1, TYPE_UNDEFINED);
        int result = push_symbol_into_table(table_stack, $1->tk_value.s, s);
        if (!is_inserted(result)) destroy_symbol(s);

        destroy_lexvalue($1); 
    } 
    | TK_IDENTIFICADOR'['lista_arranjo']'  { 
        // adiciona id na strstack para receber tipo depois
        push_strstack(strstack,$1->tk_value.s);

        // adiciona arranjo na tabela de escopo global
        symbol_t *s = create_symbol_array($1->line_number, $1, $3->tk_value.i); // dimensao total (multiplicada dependendo do tipo na add_tipos_strstack)
        int result = push_symbol_into_table(table_stack, $1->tk_value.s, s);
        if (!is_inserted(result)) destroy_symbol(s);

        destroy_lexvalue($1); 
        destroy_lexvalue($3); 
    };

lista_arranjo: 
    lista_arranjo'^'TK_LIT_INT { 
        int dim_total = $1->tk_value.i * $3->tk_value.i;
        $1->tk_value.i = dim_total;
        $$ = $1;
        destroy_lexvalue($3); 
    }
    | TK_LIT_INT { 
        $$ = $1;
    };


/* Função */
funcao:
    tipo TK_IDENTIFICADOR '(' {
        // add funcao na tabela de escopo atual
        symbol_t *s = create_symbol_function($2->line_number, $2, tktype_to_type($1->tk_type));
        int result = push_symbol_into_table(table_stack, $2->tk_value.s, s);
        if (!is_inserted(result)) destroy_symbol(s);

    } empilha_escopo func_params ')' command_block_no_new_scope  {
        // desempilha escopo do bloco de corpo da funcao

        node_t* funcao = create_node($2->tk_value.s);
        funcao->ast_type = AST_FUNCTION;
        if ($8 != NULL)
            add_child(funcao, $8);
        $$ = funcao;

        table_t *to = pop_table(table_stack);
        //print_table(to);
        destroy_table(to);
        
        destroy_lexvalue($1);
        destroy_lexvalue($2);
    };

empilha_escopo: { 
        // cria nova tabela de simbolos e empilha na pilha de escopos
        push_table(table_stack, create_symbol_table(0));
    };

func_params: lista_params {} | {};

lista_params: lista_params ',' param | param;

param: tipo TK_IDENTIFICADOR  { 
    
    // adiciona parametro como var no escopo atual (do corpo da funcao)
    symbol_t *s = create_symbol_id($2->line_number, $2, tktype_to_type($1->tk_type));    
    int result = push_symbol_into_table(table_stack, $2->tk_value.s, s);
        if (!is_inserted(result)) destroy_symbol(s);
    
    destroy_lexvalue($1);
    destroy_lexvalue($2);
};


/* Bloco de Comandos */
command_block_no_new_scope:
    '{' lista_commands '}'  { 
        $$ = $2;
    };

command_block: 
    '{' empilha_escopo lista_commands '}'  { 
        $$ = $3;
        // desempilha escopo do bloco de comando
        table_t *to = pop_table(table_stack);
        //print_table(to);
        destroy_table(to);
    };

lista_commands:
    lista_commands command ';'  { 
        if ($2 != NULL) {
            //$2 é um comando
            if ($1 != NULL) {
                node_t* last_cmd = get_last_of($1, AST_COMMAND);
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
	    $1->ast_type = AST_COMMAND;
	$$ = $1;
    } 
    | atrib {   
        $1->ast_type = AST_COMMAND;
        $$ = $1;
    } 
    | chamada_func  { 
        $1->ast_type = AST_COMMAND;
        $$ = $1;
    } 
    | retorno  { 
        $1->ast_type = AST_COMMAND;
        $$ = $1;
    } 
    | condicional  { 
        $1->ast_type = AST_COMMAND;
        $$ = $1;
    } 
    | iteracao { 
        $1->ast_type = AST_COMMAND;
        $$ = $1;
    }; 

/* Declaração de Variável */

declara_var: 
    tipo lista_local_var  {

        // desempilha strstack adicionando tipo a todos
        stack_t *ts = table_stack;
        table_t *t = get_table(ts, ts->count-1);
        add_types_to_strstack(strstack, t, $1->tk_type, 0);

        infer_type_initialization($2, tktype_to_type($1->tk_type));

        $$ = $2;
        destroy_lexvalue($1);
    };

lista_local_var:
    lista_local_var ',' local_var  {
        if ($3 != NULL) {
            if ($1 != NULL) {
                node_t* last_cmd = get_last_of($1, AST_COMMAND);
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

        // adiciona id na strstack para receber tipo depois
        push_strstack(strstack,$1->tk_value.s);

        // adiciona var na tabela de escopo atual
        symbol_t *s = create_symbol_id($1->line_number, $1, TYPE_UNDEFINED);
        int result = push_symbol_into_table(table_stack, $1->tk_value.s, s);
        if (!is_inserted(result)) destroy_symbol(s);

        destroy_lexvalue($1);
    } | 
    TK_IDENTIFICADOR TK_OC_LE literal {

        // adiciona id na strstack para receber tipo depois
        push_strstack(strstack,$1->tk_value.s);

        // adiciona var na tabela de escopo atual
        symbol_t *s = create_symbol_id($1->line_number, $1, TYPE_UNDEFINED);
        int result = push_symbol_into_table(table_stack, $1->tk_value.s, s);
        if (!is_inserted(result)) destroy_symbol(s);

        node_t* inicializa = create_node("<=");
        inicializa->ast_type = AST_COMMAND;

        node_t* var_inicializa = create_leaf($1->tk_value.s, $1);
        var_inicializa->ast_type = AST_VAR_INITIALIZED;
        add_child(inicializa, var_inicializa);

        $3->ast_type = AST_LIT_DECLARED;
        add_child(inicializa, $3);

        $$ = inicializa;

        //destroy_lexvalue($1);
    };

literal: 
    TK_LIT_INT  { 
        // adiciona literal na tabela de escopo atual
        symbol_t *s = create_symbol_literal($1->line_number, $1, TYPE_INT);
        int result = push_symbol_into_table(table_stack, $1->lexema, s);
        if (!is_inserted(result)) destroy_symbol(s);

        $$ = create_leaf($1->lexema, $1); 
        $$->sym_type = TYPE_INT;

        //destroy_lexvalue($1);
    }
    | TK_LIT_FLOAT  {
        // adiciona literal na tabela de escopo atual
        symbol_t *s = create_symbol_literal($1->line_number, $1, TYPE_FLOAT);
        int result = push_symbol_into_table(table_stack, $1->lexema, s);
        if (!is_inserted(result)) destroy_symbol(s);

        $$ = create_leaf($1->lexema, $1); 
        $$->sym_type = TYPE_FLOAT;
        //destroy_lexvalue($1);
    }
    | TK_LIT_FALSE  {
        // adiciona literal na tabela de escopo atual
        symbol_t *s = create_symbol_literal($1->line_number, $1, TYPE_BOOL);
        int result = push_symbol_into_table(table_stack, $1->lexema, s);
        if (!is_inserted(result)) destroy_symbol(s);

        $$ = create_leaf($1->lexema, $1); 
        $$->sym_type = TYPE_BOOL;
        //destroy_lexvalue($1);
    }
    | TK_LIT_TRUE   {
        // adiciona literal na tabela de escopo atual
        symbol_t *s = create_symbol_literal($1->line_number, $1, TYPE_BOOL);
        int result = push_symbol_into_table(table_stack, $1->lexema, s);
        if (!is_inserted(result)) destroy_symbol(s);

        $$ = create_leaf($1->lexema, $1); 
        $$->sym_type = TYPE_BOOL;
        //destroy_lexvalue($1);
    }
    | TK_LIT_CHAR  { 
        // adiciona literal na tabela de escopo atual
        symbol_t *s = create_symbol_literal($1->line_number, $1, TYPE_CHAR);
        int result = push_symbol_into_table(table_stack, $1->lexema, s);
        if (!is_inserted(result)) destroy_symbol(s);  

        char label[2] = "\0";
        label[0] = $1->tk_value.c;
        $$ = create_leaf(label, $1); 
        $$->sym_type = TYPE_CHAR;
        //destroy_lexvalue($1);
    };

/* Atribuição */

atrib: 
    ident_atrib '=' expr {
        node_t* atribuicao = create_node("=");
        infer_type_attribution($1->sym_type, $3);
        add_child(atribuicao, $1);
        add_child(atribuicao, $3);
        $$ = atribuicao;
    };

ident_atrib:
    TK_IDENTIFICADOR  {
        // consulta a tabela pra saber se id->sym_nature == SYM_VARIAVEL
        symbol_t* s = get_symbol_stack($1->line_number, table_stack, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_VARIAVEL);

        $$ = create_leaf($1->tk_value.s, $1);
        $$->sym_type = s->sym_type;

        //destroy_lexvalue($1);
    } 
    | TK_IDENTIFICADOR'['lista_arranjo_atrib']'  {
        // consulta a tabela pra saber se id->sym_nature == SYM_ARRANJO
        symbol_t* s = get_symbol_stack($1->line_number, table_stack, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_ARRANJO);

        node_t* id_lista_atrib = create_leaf($1->tk_value.s, $1);
        id_lista_atrib->sym_type = s->sym_type;
        node_t* indexador = create_node("[]");
        indexador->sym_type = s->sym_type;
        add_child(indexador, id_lista_atrib);
        add_child(indexador, $3);
        $$ = indexador;

        //destroy_lexvalue($1);
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

        // consulta a tabela pra saber se id->sym_nature == SYM_FUNCAO
        symbol_t* s = get_symbol_stack($1->line_number, table_stack, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_FUNCAO);

        node_t* funcao = create_node("call ");
        funcao->value = copy_lexvalue($1);
        if ($3 != NULL)
            add_child(funcao, $3);
        funcao->sym_type = s->sym_type;
        $$ = funcao;

        destroy_lexvalue($1);
    };

chamada_params: 
    chamada_lista_params  { $$ = $1; } 
    | { $$ = NULL; };

chamada_lista_params: 
    chamada_lista_params ',' expr  {
        node_t* last_expr = get_last_of($1, AST_EXPRESSION);
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

        if ($2->sym_type == TYPE_UNDEFINED) {
            printf("ERRO expressão não tem tipo");
            exit(1);
        }
        cmd_ret->sym_type = $2->sym_type;

        add_child(cmd_ret, $2);
        $$ = cmd_ret;
    };

/* Condicional */

condicional:
    TK_PR_IF '('expr')' TK_PR_THEN command_block  {
        node_t* cmd_if = create_node("if");

        if ($3->sym_type == TYPE_UNDEFINED) {
            printf("ERRO expressão não tem tipo");
            exit(1);
        }
        cmd_if->sym_type = $3->sym_type;

        add_child(cmd_if, $3);
        add_child(cmd_if, $6);
        $$ = cmd_if;
    } 
    | TK_PR_IF '('expr')' TK_PR_THEN command_block TK_PR_ELSE command_block  {
        node_t* cmd_if = create_node("if");

        if ($3->sym_type == TYPE_UNDEFINED) {
            printf("ERRO expressão não tem tipo");
            exit(1);
        }
        cmd_if->sym_type = $3->sym_type;

        add_child(cmd_if, $3);
        add_child(cmd_if, $6);
        add_child(cmd_if, $8);
        $$ = cmd_if;
    };

/* Iteração */

iteracao: 
    TK_PR_WHILE '('expr')' command_block  {
        node_t* cmd_while = create_node("while");

        if ($3->sym_type == TYPE_UNDEFINED) {
            printf("ERRO expressão não tem tipo");
            exit(1);
        }
        cmd_while->sym_type = $3->sym_type;

        add_child(cmd_while, $3);
        add_child(cmd_while, $5);
        $$ = cmd_while;
    };

/* Expressão */

lista_expr:
    lista_expr '^' expr  {
        node_t* lista = create_node("^");
        lista->sym_type = infer_type($1, $3);
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
        $$->ast_type = AST_EXPRESSION;
    };

expr_preced0: 
    expr_preced0 TK_OC_OR expr_preced1  { 
        $$ = create_node("||");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced1  { $$ = $1; };

expr_preced1: 
    expr_preced1 TK_OC_AND expr_preced2  { 
        $$ = create_node("&&");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    } 
    | expr_preced2  { $$ = $1; };

expr_preced2:
    expr_preced2 TK_OC_EQ expr_preced3  { 
        $$ = create_node("==");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced2 TK_OC_NE expr_preced3  { 
        $$ = create_node("!=");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced3  { $$ = $1; };

expr_preced3:
    expr_preced3 '<' expr_preced4  { 
        $$ = create_node("<");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    } 
    | expr_preced3 '>' expr_preced4  { 
        $$ = create_node(">");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    } 
    | expr_preced3 TK_OC_LE expr_preced4  { 
        $$ = create_node("<=");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced3 TK_OC_GE expr_preced4  { 
        $$ = create_node(">=");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    } 
    | expr_preced4  { $$ = $1; };            

expr_preced4: 
    expr_preced4 '+' expr_preced5  { 
        $$ = create_node("+");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced4 '-' expr_preced5  { 
        $$ = create_node("-");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced5  { $$ = $1; };

expr_preced5:
    expr_preced5 '*' expr_preced6  { 
        $$ = create_node("*");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced5 '/' expr_preced6  { 
        $$ = create_node("/");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced5 '%' expr_preced6  { 
        $$ = create_node("%");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
    }
    | expr_preced6  { $$ = $1; };

expr_preced6: 
    '-' expr_terminais  { 
        $$ = create_node("-");
        $$->sym_type = $2->sym_type;
        add_child($$, $2);
    }
    | '!' expr_terminais  { 
        $$ = create_node("!");
        $$->sym_type = $2->sym_type;
        add_child($$, $2);
    }
    | expr_terminais { $$ = $1; };

expr_terminais: 
    TK_IDENTIFICADOR  { 

        // consulta a tabela pra saber se id->sym_nature == SYM_VARIAVEL
        symbol_t* s = get_symbol_stack($1->line_number, table_stack, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_VARIAVEL);

        node_t* id = create_leaf($1->tk_value.s, $1);
        $$ = id;
        $$->sym_type = s->sym_type;

        iloc_code_t* code_loadAI;
        int r = new_reg();
        // se global usa rbss, senao rfp
        if (s->global == 1) {
            code_loadAI = create_iloc_code3op(LOAD_AI, IMMEDIATE, rbss, IMMEDIATE, s->disp, TEMPORARY, r);
        } else {
            code_loadAI = create_iloc_code3op(LOAD_AI, IMMEDIATE, rfp, IMMEDIATE, s->disp, TEMPORARY, r);
        }
        id->code = create_iloc_program();
        push_iloc_code(id->code, code_loadAI);
        print_program(id->code);

        //destroy_lexvalue($1);
    }
    | TK_IDENTIFICADOR '[' lista_expr ']'  {

        // consulta a tabela pra saber se id->sym_nature == SYM_ARRANJO
        symbol_t* s = get_symbol_stack($1->line_number, table_stack, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_ARRANJO);

        node_t* id_lista_expr = create_leaf($1->tk_value.s, $1);
        id_lista_expr->sym_type = s->sym_type;
        node_t* indexador = create_node("[]");
        indexador->sym_type = s->sym_type;
        add_child(indexador, id_lista_expr);
        add_child(indexador, $3);
        $$ = indexador;

        //destroy_lexvalue($1);
    }
    | literal  { $$ = $1; }
    | '(' expr ')'  { $$ = $2; }
    | chamada_func  { $$ = $1; };

%%


void yyerror(char const *s) {

  printf("Erro Sintático [%s] na linha %d\n", s, yylineno);
  return; 


}



