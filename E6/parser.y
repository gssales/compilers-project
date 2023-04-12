%{
#include <stdio.h>
#include <stdbool.h>

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
    #include "stringlist.h"
    extern void* arvore;
    extern void* table_stack;
    extern void* strlist;
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

        // table_t *to = pop_table(table_stack);
        //print_table(to);

        clear_strlist(strlist);
        free(strlist);

        //push_iloc_code($2->code, create_iloc_code(HALT));

        //symbol_t* main = get_symbol_stack(0, table_stack, "main")->symbol;
        //iloc_program_t* program = create_iloc_program();
        //push_iloc_code(program, create_iloc_code2op(LOAD_I, IMMEDIATE, 1024, TEMPORARY, ILOC_RFP));
        //push_iloc_code(program, create_iloc_code2op(LOAD_I, IMMEDIATE, 1024, TEMPORARY, ILOC_RSP));
        //push_iloc_code(program, create_iloc_code2op(LOAD_I, IMMEDIATE, $2->code->count + 6, TEMPORARY, ILOC_RBSS));
        //push_iloc_code(program, create_iloc_code2op(I2I, TEMPORARY, ILOC_RFP, TEMPORARY, ILOC_RSP));
        //push_iloc_code(program, create_iloc_code1op(JUMP_I, LABEL, main->label));
        //concat_iloc_program(program, $2->code);
        //push_iloc_code(program, create_iloc_code(HALT));
        //print_program(program);

        //destroy_stack(table_stack);
        //destroy_iloc_program(program);
    } 
    | {
        arvore = NULL;
    };

escopo_global: {
    // cria pilha e empilha a tabela de escopo global
    table_stack = create_stack();
    push_table(table_stack, create_symbol_table(1, 0, 0));

    // cria lista de identificadores para receberem tipo/tamanho/deslocamento nas declaracoes
    strlist = create_strlist();
}

lista_elementos: 
    lista_elementos elemento  {
        if ($2 != NULL) {
            if ($1 != NULL) {
                node_t* last_function = get_last_of($1, AST_FUNCTION);
                add_child(last_function, $2);
                concat_iloc_program($1->code, $2->code);
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

    stack_t* ts = table_stack;
    table_t *t = get_table(ts, ts->count-1);

    // adiciona tipo/tamanho/deslocamento a lista de declaracoes
    add_types_to_strlist(strlist, t, $1->tk_type);
    clear_strlist(strlist);

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

        // adiciona id na strlist para receber tipo/tam/deslocamento depois
        add_strlist(strlist,$1->tk_value.s);

        // adiciona simbolo na tabela de escopo global
        symbol_t *s = create_symbol_id($1->line_number, $1, TYPE_UNDEFINED);
        int result = push_symbol_into_table(table_stack, $1->tk_value.s, s);
        if (!is_inserted(result)) destroy_symbol(s);

        destroy_lexvalue($1); 
    } 
    | TK_IDENTIFICADOR'['lista_arranjo']'  { 
        // adiciona id na strlist para receber tipo/tam/deslocamento depois
        add_strlist(strlist,$1->tk_value.s);

        // adiciona arranjo na tabela de escopo global
        symbol_t *s = create_symbol_array($1->line_number, $1, $3->tk_value.i);
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
        int rot = new_label();
        s->label = rot;
        int result = push_symbol_into_table(table_stack, $2->tk_value.s, s);
        if (!is_inserted(result)) destroy_symbol(s);

        if (strcmp($2->tk_value.s, "main") == 0) {
            table_t* new_table = create_symbol_table(0, 1, 0);
            new_table->end_label = new_label();
            push_table(table_stack, new_table);
        }
        else
            push_table(table_stack, create_symbol_table(0, 0, 16));


    } func_params ')' command_block_no_new_scope  {
        // desempilha escopo do bloco de corpo da funcao

        node_t* funcao = create_node($2->tk_value.s);
        funcao->ast_type = AST_FUNCTION;
        if ($7 != NULL)
            add_child(funcao, $7);
        $$ = funcao;

        table_t *to = pop_table(table_stack);
        symbol_t *s = get_symbol_stack($2->line_number, table_stack, $2->tk_value.s)->symbol;

        iloc_program_t* p = create_iloc_program();
        if (!to->is_main_function) {
            push_iloc_code(p, create_iloc_code2op(I2I, TEMPORARY, ILOC_RSP, TEMPORARY, ILOC_RFP));
        }
        push_iloc_code(p, create_iloc_code3op(ADD_I, TEMPORARY, ILOC_RSP, IMMEDIATE, to->disp, TEMPORARY, ILOC_RSP));
        p->tail->is_move_sp = true;
        p->tail->has_call = to->has_call;

        if ($7 && $7->code && $7->code->count > 0) {
          concat_iloc_program(p, $7->code);
          // destroy_iloc_program($7->code);
        }
        if (to->is_main_function) {
            iloc_code_t* end = create_iloc_code(NOP);
            end->label = to->end_label;
            push_iloc_code(p, end);
        }
        p->head->label = s->label;
        p->head->is_start_function = 1;
        p->head->asm_label = s->value->tk_value.s;
        p->tail->is_end_function = 1;
        $$->code = p;

        //print_table(to);
        destroy_table(to);
        
        destroy_lexvalue($1);
        destroy_lexvalue($2);
    };

empilha_escopo: { 
        // cria nova tabela de simbolos e empilha na pilha de escopos
        push_table(table_stack, create_symbol_table(0, 0, 16));
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
                concat_iloc_program($1->code, $2->code);
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

        stack_t *ts = table_stack;
        table_t *t = get_table(ts, ts->count-1);

        // adiciona tipo/tamanho/deslocamento a lista de declaracoes
        add_types_to_strlist(strlist, t, $1->tk_type);
        clear_strlist(strlist);

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
                concat_iloc_program($1->code, $3->code);
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

        // adiciona id na strlist para receber tipo/tam/deslocamento depois
        add_strlist(strlist,$1->tk_value.s);

        // adiciona var na tabela de escopo atual
        symbol_t *s = create_symbol_id($1->line_number, $1, TYPE_UNDEFINED);
        int result = push_symbol_into_table(table_stack, $1->tk_value.s, s);
        if (!is_inserted(result)) destroy_symbol(s);

        destroy_lexvalue($1);
    } | 
    TK_IDENTIFICADOR TK_OC_LE literal {

        // adiciona id na strlist para receber tipo/tam/deslocamento depois
        add_strlist(strlist,$1->tk_value.s);

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

        // geracao de codigo
        iloc_code_t* code_storeAI; // codigo a ser gerado
        int r2 = $3->tmp; // temporario com literal
        // verifica se global (rbss) ou local (rfp)
        if (s->global) {
            code_storeAI = create_iloc_code3op(STORE_AI, TEMPORARY, r2, TEMPORARY, ILOC_RBSS, IMMEDIATE, s->disp);
            code_storeAI->asm_label = s->value->tk_value.s;
        } else {
            code_storeAI = create_iloc_code3op(STORE_AI, TEMPORARY, r2, TEMPORARY, ILOC_RFP, IMMEDIATE, s->disp);
        }

        // concatena codigo do literal com codigo gerado
        $$->code = create_iloc_program();
        concat_iloc_program($$->code, $3->code);
        push_iloc_code($$->code, code_storeAI);
        //print_program($$->code); // debug

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

        $$->code = create_iloc_program(); // apenas int suportado
        $$->tmp = new_reg();
        push_iloc_code($$->code, create_iloc_code2op(LOAD_I, IMMEDIATE, $1->tk_value.i, TEMPORARY, $$->tmp));

        //destroy_lexvalue($1);
    }
    | TK_LIT_FLOAT  {
        // adiciona literal na tabela de escopo atual
        symbol_t *s = create_symbol_literal($1->line_number, $1, TYPE_FLOAT);
        int result = push_symbol_into_table(table_stack, $1->lexema, s);
        if (!is_inserted(result)) destroy_symbol(s);

        $$ = create_leaf($1->lexema, $1); 
        $$->sym_type = TYPE_FLOAT;

        $$->code = NULL; // tipo nao suportado na geracao de codigo

        //destroy_lexvalue($1);
    }
    | TK_LIT_FALSE  {
        // adiciona literal na tabela de escopo atual
        symbol_t *s = create_symbol_literal($1->line_number, $1, TYPE_BOOL);
        int result = push_symbol_into_table(table_stack, $1->lexema, s);
        if (!is_inserted(result)) destroy_symbol(s);

        $$ = create_leaf($1->lexema, $1); 
        $$->sym_type = TYPE_BOOL;

        $$->code = create_iloc_program(); // apenas int suportado
        $$->tmp = new_reg();
        push_iloc_code($$->code, create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, $$->tmp));
        
        //destroy_lexvalue($1);
    }
    | TK_LIT_TRUE   {
        // adiciona literal na tabela de escopo atual
        symbol_t *s = create_symbol_literal($1->line_number, $1, TYPE_BOOL);
        int result = push_symbol_into_table(table_stack, $1->lexema, s);
        if (!is_inserted(result)) destroy_symbol(s);

        $$ = create_leaf($1->lexema, $1); 
        $$->sym_type = TYPE_BOOL;

        $$->code = create_iloc_program(); // apenas int suportado
        $$->tmp = new_reg();
        push_iloc_code($$->code, create_iloc_code2op(LOAD_I, IMMEDIATE, 1, TEMPORARY, $$->tmp));

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

        $$->code = NULL; // tipo nao suportado na geracao de codigo

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

        symbol_t* s = get_symbol_stack($1->value->line_number, table_stack, $1->value->tk_value.s)->symbol;

        // geracao de codigo
        iloc_code_t* code_storeAI; // codigo a ser gerado
        int r = $3->tmp; // temporario com resultado da expr
        // verifica se SYM_VARIAVEL (SYM_ARRANJO nao suportado)
        if (s->sym_nature == SYM_VARIAVEL) {
            // verifica se global (rbss) ou local (rfp)
            if (s->global) {
                code_storeAI = create_iloc_code3op(STORE_AI, TEMPORARY, r, TEMPORARY, ILOC_RBSS, IMMEDIATE, s->disp);
                code_storeAI->asm_label = s->value->tk_value.s;
            } else {
                code_storeAI = create_iloc_code3op(STORE_AI, TEMPORARY, r, TEMPORARY, ILOC_RFP, IMMEDIATE, s->disp);
            }
        }

        // concatena codigo da expr com codigo gerado
        $$->code = create_iloc_program();
        concat_iloc_program($$->code, $3->code);
        push_iloc_code($$->code, code_storeAI);
        //print_program($$->code); // debug
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

        table_t* t = get_table(table_stack, -1);
        t->has_call = true;

        // consulta a tabela pra saber se id->sym_nature == SYM_FUNCAO
        symbol_t* s = get_symbol_stack($1->line_number, table_stack, $1->tk_value.s)->symbol;
        check_correct_use($1->line_number, s, SYM_FUNCAO);

        node_t* funcao = create_node("call ");
        funcao->value = copy_lexvalue($1);
        if ($3 != NULL)
            add_child(funcao, $3);
        funcao->sym_type = s->sym_type;
        $$ = funcao;
        
        iloc_program_t* p = create_iloc_program();
        int tmp = new_reg();
        int jump_rpc = 0;
        if ($3 != NULL) {
          jump_rpc = $3->count_tmpList;
          concat_iloc_program(p, $3->code);
        }
        push_iloc_code(p, create_iloc_code3op(ADD_I, TEMPORARY, ILOC_RPC, IMMEDIATE, jump_rpc + 5/* endereço de retorno */, TEMPORARY, tmp));
        p->tail->discard = 1;
        push_iloc_code(p, create_iloc_code3op(STORE_AI, TEMPORARY, tmp, TEMPORARY, ILOC_RSP, IMMEDIATE, 0));
        p->tail->discard = 1;
        push_iloc_code(p, create_iloc_code3op(STORE_AI, TEMPORARY, ILOC_RSP, TEMPORARY, ILOC_RSP, IMMEDIATE, 4));
        p->tail->discard = 1;
        push_iloc_code(p, create_iloc_code3op(STORE_AI, TEMPORARY, ILOC_RFP, TEMPORARY, ILOC_RSP, IMMEDIATE, 8));
        p->tail->discard = 1;
        if ($3 != NULL) {
          for (int i = 0; i < $3->count_tmpList; i++) {
            push_iloc_code(p, create_iloc_code3op(STORE_AI, TEMPORARY, $3->tmpList[i], TEMPORARY, ILOC_RSP, IMMEDIATE, 16 + i*4));
          }
        }
        push_iloc_code(p, create_iloc_code1op(JUMP_I, LABEL, s->label));
        p->tail->is_call_function = 1;
        p->tail->asm_label = s->value->tk_value.s;
        push_iloc_code(p, create_iloc_code3op(LOAD_AI, TEMPORARY, ILOC_RSP, IMMEDIATE, 12, TEMPORARY, tmp));
        p->tail->is_get_retval = 1;

        $$->code = p;
        $$->tmp = tmp;

        // print_program(p);

        destroy_lexvalue($1);
    };

chamada_params: 
    chamada_lista_params  { $$ = $1; } 
    | { $$ = NULL; };

chamada_lista_params: 
    chamada_lista_params ',' expr  {
        node_t* last_expr = get_last_of($1, AST_EXPRESSION);
        add_child(last_expr, $3);
        concat_iloc_program($1->code, $3->code);
        $1->count_tmpList++;
        $1->tmpList = realloc($1->tmpList, $1->count_tmpList * sizeof(int));
        $1->tmpList[$1->count_tmpList-1] = $3->tmp;
        $$ = $1;
    } | 
    expr  {
        $1->count_tmpList = 1;
        $1->tmpList = malloc($1->count_tmpList * sizeof(int));
        $1->tmpList[0] = $1->tmp;
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

        stack_t *ts = table_stack;
        table_t* current_table = get_table(ts, -1);
        iloc_program_t* p = create_iloc_program();

        concat_iloc_program(p, $2->code);
        if (current_table->is_main_function) {
            iloc_code_t* retval = create_iloc_code3op(STORE_AI, TEMPORARY, $2->tmp, TEMPORARY, ILOC_RFP, IMMEDIATE, 0);
            retval->is_retval = 1;
            push_iloc_code(p, retval);

            iloc_code_t* ret = create_iloc_code1op(JUMP_I, LABEL, current_table->end_label);
            ret->is_ret = 1;
            ret->has_call = current_table->has_call;
            push_iloc_code(p, ret);
        } else {
            iloc_code_t* retval = create_iloc_code3op(STORE_AI, TEMPORARY, $2->tmp, TEMPORARY, ILOC_RFP, IMMEDIATE, 12);
            retval->is_retval = 1;
            push_iloc_code(p, retval);
            
            int end_retorno = new_reg();
            push_iloc_code(p, create_iloc_code3op(LOAD_AI, TEMPORARY, ILOC_RFP, IMMEDIATE, 0, TEMPORARY, end_retorno));
            p->tail->discard = 1;
            int rsp_restore = new_reg();
            push_iloc_code(p, create_iloc_code3op(LOAD_AI, TEMPORARY, ILOC_RFP, IMMEDIATE, 4, TEMPORARY, rsp_restore));
            p->tail->discard = 1;
            int rfp_restore = new_reg();
            push_iloc_code(p, create_iloc_code3op(LOAD_AI, TEMPORARY, ILOC_RFP, IMMEDIATE, 8, TEMPORARY, rfp_restore));
            p->tail->discard = 1;
            push_iloc_code(p, create_iloc_code2op(I2I, TEMPORARY, rsp_restore, TEMPORARY, ILOC_RSP));
            p->tail->discard = 1;
            push_iloc_code(p, create_iloc_code2op(I2I, TEMPORARY, rfp_restore, TEMPORARY, ILOC_RFP));
            p->tail->discard = 1;

            iloc_code_t* ret = create_iloc_code1op(JUMP, TEMPORARY, end_retorno);
            ret->is_ret = 1;
            ret->has_call = current_table->has_call;
            push_iloc_code(p, ret);
        }
        cmd_ret->code = p;
        // print_program(p);

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

        // geracao de codigo
        int r = new_reg();    // temporario
        int r2 = new_reg();   // temporario opaco
        int lt = new_label(); // label_verdade
        int ld = new_label(); // label_depois

        iloc_code_t* code_loadi;
        code_loadi = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, r);

        iloc_code_t* code_cmpne;
        code_cmpne = create_iloc_code3op(CMP_NE, TEMPORARY, $3->tmp, TEMPORARY, r, TEMPORARY, r2);

        iloc_code_t* code_cbr;
        code_cbr = create_iloc_code3op(CBR, TEMPORARY, r2, LABEL, lt, LABEL, ld);

        iloc_code_t* code_nopt;
        code_nopt = create_iloc_code(NOP);
        code_nopt->label = lt;

        // $6.code

        iloc_code_t* code_jumpI;
        code_jumpI = create_iloc_code1op(JUMP_I, LABEL, ld);

        iloc_code_t* code_nopd;
        code_nopd = create_iloc_code(NOP);
        code_nopd->label = ld;

        // concatena codigo gerado e blocos
        $$->code = create_iloc_program();

        concat_iloc_program($$->code, $3->code);
        push_iloc_code($$->code, code_loadi);
        push_iloc_code($$->code, code_cmpne);
        push_iloc_code($$->code, code_cbr);
        push_iloc_code($$->code, code_nopt);
        concat_iloc_program($$->code, $6->code);
        push_iloc_code($$->code, code_jumpI);
        push_iloc_code($$->code, code_nopd);

        //print_program($$->code); // debug
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

        // geracao de codigo
        int r = new_reg();    // temporario
        int r2 = new_reg();   // temporario opaco
        int lt = new_label(); // label_verdade
        int lf = new_label(); // label_falso
        int ld = new_label(); // label_depois

        iloc_code_t* code_loadi;
        code_loadi = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, r);

        iloc_code_t* code_cmpne;
        code_cmpne = create_iloc_code3op(CMP_NE, TEMPORARY, $3->tmp, TEMPORARY, r, TEMPORARY, r2);

        iloc_code_t* code_cbr;
        code_cbr = create_iloc_code3op(CBR, TEMPORARY, r2, LABEL, lt, LABEL, lf);

        iloc_code_t* code_nopt;
        code_nopt = create_iloc_code(NOP);
        code_nopt->label = lt;

        // $6.code

        iloc_code_t* code_jumpI;
        code_jumpI = create_iloc_code1op(JUMP_I, LABEL, ld);

        iloc_code_t* code_nopf;
        code_nopf = create_iloc_code(NOP);
        code_nopf->label = lf;

        // $8.code

        iloc_code_t* code_nopd;
        code_nopd = create_iloc_code(NOP);
        code_nopd->label = ld;

        // concatena codigo gerado e blocos
        $$->code = create_iloc_program();

        concat_iloc_program($$->code, $3->code);
        push_iloc_code($$->code, code_loadi);
        push_iloc_code($$->code, code_cmpne);
        push_iloc_code($$->code, code_cbr);
        push_iloc_code($$->code, code_nopt);
        concat_iloc_program($$->code, $6->code);
        push_iloc_code($$->code, code_jumpI);
        push_iloc_code($$->code, code_nopf);
        concat_iloc_program($$->code, $8->code);
        push_iloc_code($$->code, code_nopd);

        //print_program($$->code); // debug
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
        
        // geracao de codigo
        int r = new_reg();    // temporario
        int r2 = new_reg();   // temporario opaco
        int lb_cond = new_label(); // lb_condicao
        int lb_block = new_label(); // lb_codigo
        int lb_depois = new_label(); // lb_depois

        // lb_codigo: nop + codigo do command_block ($5->code)
        iloc_code_t* code_block_nop;
        code_block_nop = create_iloc_code(NOP);
        code_block_nop->label = lb_block;

        // lb_cond: codigo do teste condicao expr (loadi+cmpne+cbr)
        // iloc_code_t* code_loadi;
        // code_loadi = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, r);
        // code_loadi->label = lb_cond;
        // iloc_code_t* code_cmpne;
        // code_cmpne = create_iloc_code3op(CMP_NE, TEMPORARY, $3->tmp, TEMPORARY, r, TEMPORARY, r2);
        iloc_code_t* code_cbr;
        code_cbr = create_iloc_code3op(CBR, TEMPORARY, $3->tmp, LABEL, lb_block, LABEL, lb_depois);

        // lb_depois: nop
        iloc_code_t* code_nopd;
        code_nopd = create_iloc_code(NOP);
        code_nopd->label = lb_depois;

        // concatena codigo gerado e bloco
        $$->code = create_iloc_program();
        $3->code->head->label = lb_cond;
        concat_iloc_program($$->code, $3->code);
        push_iloc_code($$->code, code_cbr);

        // TODO: DEBUG CODE_BLOCK NAO SENDO INSERIDO?
        push_iloc_code($$->code, code_block_nop); // lb_block
        concat_iloc_program($$->code, $5->code);

        push_iloc_code($$->code, create_iloc_code1op(JUMP_I, LABEL, lb_cond));
        push_iloc_code($$->code, code_nopd); // lb_depois
        // push_iloc_code($$->code, code_loadi); // lb_cond
        // push_iloc_code($$->code, code_cmpne);

        // print_program($$->code); // debug

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

        // geracao de codigo
        iloc_code_t* code_or;
        code_or = create_iloc_code3op(OR, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, $3->tmp);

        // concatena codigo das expr e add codigo gerado
        $$->code = create_iloc_program();
        concat_iloc_program($$->code, $1->code);
        concat_iloc_program($$->code, $3->code);
        push_iloc_code($$->code, code_or);

        // temporario com resultado
        $$->tmp = $3->tmp;

        //print_program($$->code); // debug
    }
    | expr_preced1  { $$ = $1; };

expr_preced1: 
    expr_preced1 TK_OC_AND expr_preced2  { 
        $$ = create_node("&&");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        iloc_code_t* code_and;
        code_and = create_iloc_code3op(AND, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, $3->tmp);

        // concatena codigo das expr e add codigo gerado
        $$->code = create_iloc_program();
        concat_iloc_program($$->code, $1->code);
        concat_iloc_program($$->code, $3->code);
        push_iloc_code($$->code, code_and);

        // temporario com resultado
        $$->tmp = $3->tmp;

        //print_program($$->code); // debug
    } 
    | expr_preced2  { $$ = $1; };

expr_preced2:
    expr_preced2 TK_OC_EQ expr_preced3  { 
        $$ = create_node("==");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        int r = new_reg();    // temporario
        int lt = new_label(); // label_verdade
        int lf = new_label(); // label_falso
        int ld = new_label(); // label_depois

        iloc_code_t* code_cmpeq;
        code_cmpeq = create_iloc_code3op(CMP_EQ, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, r);

        iloc_code_t* code_cbr;
        code_cbr = create_iloc_code3op(CBR, TEMPORARY, r, LABEL, lt, LABEL, lf);

        iloc_code_t* code_loadi_lt;
        code_loadi_lt = create_iloc_code2op(LOAD_I, IMMEDIATE, 1, TEMPORARY, r);
        code_loadi_lt->label = lt;

        iloc_code_t* code_jumpI;
        code_jumpI = create_iloc_code1op(JUMP_I, LABEL, ld);

        iloc_code_t* code_loadi_lf;
        code_loadi_lf = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, r);
        code_loadi_lf->label = lf;

        iloc_code_t* code_nop;
        code_nop = create_iloc_code(NOP);
        code_nop->label = ld;

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);

        push_iloc_code(p, code_cmpeq);
        push_iloc_code(p, code_cbr);
        push_iloc_code(p, code_loadi_lt);
        push_iloc_code(p, code_jumpI);
        push_iloc_code(p, code_loadi_lf);
        push_iloc_code(p, code_nop);

        $$->code = p;
        $$->tmp = r;

        //print_program($$->code); // debug
    }
    | expr_preced2 TK_OC_NE expr_preced3  { 
        $$ = create_node("!=");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        int r = new_reg();    // temporario
        int lt = new_label(); // label_verdade
        int lf = new_label(); // label_falso
        int ld = new_label(); // label_depois

        iloc_code_t* code_cmpne;
        code_cmpne = create_iloc_code3op(CMP_NE, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, r);

        iloc_code_t* code_cbr;
        code_cbr = create_iloc_code3op(CBR, TEMPORARY, r, LABEL, lt, LABEL, lf);

        iloc_code_t* code_loadi_lt;
        code_loadi_lt = create_iloc_code2op(LOAD_I, IMMEDIATE, 1, TEMPORARY, r);
        code_loadi_lt->label = lt;

        iloc_code_t* code_jumpI;
        code_jumpI = create_iloc_code1op(JUMP_I, LABEL, ld);

        iloc_code_t* code_loadi_lf;
        code_loadi_lf = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, r);
        code_loadi_lf->label = lf;

        iloc_code_t* code_nop;
        code_nop = create_iloc_code(NOP);
        code_nop->label = ld;

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);

        push_iloc_code(p, code_cmpne);
        push_iloc_code(p, code_cbr);
        push_iloc_code(p, code_loadi_lt);
        push_iloc_code(p, code_jumpI);
        push_iloc_code(p, code_loadi_lf);
        push_iloc_code(p, code_nop);

        $$->code = p;
        $$->tmp = r;

        //print_program($$->code); // debug
    }
    | expr_preced3  { $$ = $1; };

expr_preced3:
    expr_preced3 '<' expr_preced4  { 
        $$ = create_node("<");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        int r = new_reg();    // temporario
        int lt = new_label(); // label_verdade
        int lf = new_label(); // label_falso
        int ld = new_label(); // label_depois

        iloc_code_t* code_cmplt;
        code_cmplt = create_iloc_code3op(CMP_LT, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, r);

        iloc_code_t* code_cbr;
        code_cbr = create_iloc_code3op(CBR, TEMPORARY, r, LABEL, lt, LABEL, lf);

        iloc_code_t* code_loadi_lt;
        code_loadi_lt = create_iloc_code2op(LOAD_I, IMMEDIATE, 1, TEMPORARY, r);
        code_loadi_lt->label = lt;

        iloc_code_t* code_jumpI;
        code_jumpI = create_iloc_code1op(JUMP_I, LABEL, ld);

        iloc_code_t* code_loadi_lf;
        code_loadi_lf = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, r);
        code_loadi_lf->label = lf;

        iloc_code_t* code_nop;
        code_nop = create_iloc_code(NOP);
        code_nop->label = ld;

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);

        push_iloc_code(p, code_cmplt);
        push_iloc_code(p, code_cbr);
        push_iloc_code(p, code_loadi_lt);
        push_iloc_code(p, code_jumpI);
        push_iloc_code(p, code_loadi_lf);
        push_iloc_code(p, code_nop);

        $$->code = p;
        $$->tmp = r;

        //print_program($$->code); // debug
    } 
    | expr_preced3 '>' expr_preced4  { 
        $$ = create_node(">");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        int r = new_reg();    // temporario
        int lt = new_label(); // label_verdade
        int lf = new_label(); // label_falso
        int ld = new_label(); // label_depois

        iloc_code_t* code_cmpgt;
        code_cmpgt = create_iloc_code3op(CMP_GT, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, r);

        iloc_code_t* code_cbr;
        code_cbr = create_iloc_code3op(CBR, TEMPORARY, r, LABEL, lt, LABEL, lf);

        iloc_code_t* code_loadi_lt;
        code_loadi_lt = create_iloc_code2op(LOAD_I, IMMEDIATE, 1, TEMPORARY, r);
        code_loadi_lt->label = lt;

        iloc_code_t* code_jumpI;
        code_jumpI = create_iloc_code1op(JUMP_I, LABEL, ld);

        iloc_code_t* code_loadi_lf;
        code_loadi_lf = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, r);
        code_loadi_lf->label = lf;

        iloc_code_t* code_nop;
        code_nop = create_iloc_code(NOP);
        code_nop->label = ld;

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);

        push_iloc_code(p, code_cmpgt);
        push_iloc_code(p, code_cbr);
        push_iloc_code(p, code_loadi_lt);
        push_iloc_code(p, code_jumpI);
        push_iloc_code(p, code_loadi_lf);
        push_iloc_code(p, code_nop);

        $$->code = p;
        $$->tmp = r;

        //print_program($$->code); // debug
    } 
    | expr_preced3 TK_OC_LE expr_preced4  { 
        $$ = create_node("<=");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        int r = new_reg();    // temporario
        int lt = new_label(); // label_verdade
        int lf = new_label(); // label_falso
        int ld = new_label(); // label_depois

        iloc_code_t* code_cmple;
        code_cmple = create_iloc_code3op(CMP_LE, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, r);

        iloc_code_t* code_cbr;
        code_cbr = create_iloc_code3op(CBR, TEMPORARY, r, LABEL, lt, LABEL, lf);

        iloc_code_t* code_loadi_lt;
        code_loadi_lt = create_iloc_code2op(LOAD_I, IMMEDIATE, 1, TEMPORARY, r);
        code_loadi_lt->label = lt;

        iloc_code_t* code_jumpI;
        code_jumpI = create_iloc_code1op(JUMP_I, LABEL, ld);

        iloc_code_t* code_loadi_lf;
        code_loadi_lf = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, r);
        code_loadi_lf->label = lf;

        iloc_code_t* code_nop;
        code_nop = create_iloc_code(NOP);
        code_nop->label = ld;

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);

        push_iloc_code(p, code_cmple);
        push_iloc_code(p, code_cbr);
        push_iloc_code(p, code_loadi_lt);
        push_iloc_code(p, code_jumpI);
        push_iloc_code(p, code_loadi_lf);
        push_iloc_code(p, code_nop);

        $$->code = p;
        $$->tmp = r;

        //print_program($$->code); // debug
    }
    | expr_preced3 TK_OC_GE expr_preced4  { 
        $$ = create_node(">=");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        int r = new_reg();    // temporario
        int lt = new_label(); // label_verdade
        int lf = new_label(); // label_falso
        int ld = new_label(); // label_depois

        iloc_code_t* code_cmpge;
        code_cmpge = create_iloc_code3op(CMP_GE, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, r);

        iloc_code_t* code_cbr;
        code_cbr = create_iloc_code3op(CBR, TEMPORARY, r, LABEL, lt, LABEL, lf);

        iloc_code_t* code_loadi_lt;
        code_loadi_lt = create_iloc_code2op(LOAD_I, IMMEDIATE, 1, TEMPORARY, r);
        code_loadi_lt->label = lt;

        iloc_code_t* code_jumpI;
        code_jumpI = create_iloc_code1op(JUMP_I, LABEL, ld);

        iloc_code_t* code_loadi_lf;
        code_loadi_lf = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, r);
        code_loadi_lf->label = lf;

        iloc_code_t* code_nop;
        code_nop = create_iloc_code(NOP);
        code_nop->label = ld;

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);

        push_iloc_code(p, code_cmpge);
        push_iloc_code(p, code_cbr);
        push_iloc_code(p, code_loadi_lt);
        push_iloc_code(p, code_jumpI);
        push_iloc_code(p, code_loadi_lf);
        push_iloc_code(p, code_nop);

        $$->code = p;
        $$->tmp = r;

        //print_program($$->code); // debug
    } 
    | expr_preced4  { $$ = $1; };            

expr_preced4: 
    expr_preced4 '+' expr_preced5  { 
        $$ = create_node("+");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
        
        // geracao de codigo
        iloc_code_t* code_add;
        code_add = create_iloc_code3op(ADD, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, $3->tmp);

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);
        push_iloc_code(p, code_add);
        $$->code = p;

        // temporario com resultado
        $$->tmp = $3->tmp;
        
        //print_program($$->code); // debug

    }
    | expr_preced4 '-' expr_preced5  { 
        $$ = create_node("-");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        iloc_code_t* code_sub;
        code_sub = create_iloc_code3op(SUB, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, $3->tmp);

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);
        push_iloc_code(p, code_sub);
        $$->code = p;

        // temporario com resultado
        $$->tmp = $3->tmp;

        //print_program($$->code); // debug
    }
    | expr_preced5  { $$ = $1; };

expr_preced5:
    expr_preced5 '*' expr_preced6  { 
        $$ = create_node("*");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        iloc_code_t* code_mult;
        code_mult = create_iloc_code3op(MULT, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, $3->tmp);

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);
        push_iloc_code(p, code_mult);
        $$->code = p;
        
        // temporario com resultado
        $$->tmp = $3->tmp;

        //print_program($$->code); // debug
    }
    | expr_preced5 '/' expr_preced6  { 
        $$ = create_node("/");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);

        // geracao de codigo
        iloc_code_t* code_div;
        int r = new_reg();
        code_div = create_iloc_code3op(DIV, TEMPORARY, $1->tmp, TEMPORARY, $3->tmp, TEMPORARY, r);

        // concatena codigo das expr e add codigo gerado
        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $1->code);
        concat_iloc_program(p, $3->code);
        push_iloc_code(p, code_div);
        $$->code = p;
        // temporario com resultado
        $$->tmp = r;

        //print_program($$->code); // debug
    }
    | expr_preced5 '%' expr_preced6  { 
        $$ = create_node("%");
        $$->sym_type = infer_type($1, $3);
        add_child($$, $1);
        add_child($$, $3);
        destroy_iloc_program($1->code);
        destroy_iloc_program($3->code);
    }
    | expr_preced6  { $$ = $1; };

expr_preced6: 
    '-' expr_terminais  { 
        $$ = create_node("-");
        $$->sym_type = $2->sym_type;
        add_child($$, $2);

        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $2->code);
        push_iloc_code(p, create_iloc_code3op(R_SUB_I, TEMPORARY, $2->tmp, IMMEDIATE, 0, TEMPORARY, $2->tmp));
        $$->tmp = $2->tmp;
        $$->code = p;
        //destroy_iloc_program($2->code);
    }
    | '!' expr_terminais  { 
        $$ = create_node("!");
        $$->sym_type = $2->sym_type;
        add_child($$, $2);

        iloc_program_t* p = create_iloc_program();
        concat_iloc_program(p, $2->code);
        int ltrue = new_label();
        int lfalse = new_label();
        int lcontinue = new_label();
        push_iloc_code(p, create_iloc_code3op(CBR, TEMPORARY, $2->tmp, LABEL, ltrue, LABEL, lfalse));

        iloc_code_t* code_true = create_iloc_code2op(LOAD_I, IMMEDIATE, 0, TEMPORARY, $2->tmp);
        code_true->label = ltrue;
        push_iloc_code(p, code_true);
        push_iloc_code(p, create_iloc_code1op(JUMP_I, LABEL, lcontinue));        

        iloc_code_t* code_false = create_iloc_code2op(LOAD_I, IMMEDIATE, 1, TEMPORARY, $2->tmp);
        code_false->label = lfalse;
        push_iloc_code(p, code_false);

        iloc_code_t* code_continue = create_iloc_code(NOP);
        code_continue->label = lcontinue;
        push_iloc_code(p, code_continue);

        $$->tmp = $2->tmp;
        $$->code = p;
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

        // geracao de codigo
        iloc_code_t* code_loadAI;
        int r = new_reg();
        // global usa rbss, local usa rfp
        if (s->global) {
            code_loadAI = create_iloc_code3op(LOAD_AI, TEMPORARY, ILOC_RBSS, IMMEDIATE, s->disp, TEMPORARY, r);
            code_loadAI->asm_label = s->value->tk_value.s;
        } else {
            code_loadAI = create_iloc_code3op(LOAD_AI, TEMPORARY, ILOC_RFP, IMMEDIATE, s->disp, TEMPORARY, r);
        }

        // add codigo no node
        $$->code = create_iloc_program();
        push_iloc_code($$->code, code_loadAI);

        // temporario com resultado
        $$->tmp = r;

        //print_program(id->code); // debug

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
    | literal       { $$ = $1; }
    | '(' expr ')'  { $$ = $2; }
    | chamada_func  { $$ = $1; };

%%


void yyerror(char const *s) {

  printf("Erro Sintático [%s] na linha %d\n", s, yylineno);
  return; 


}