#ifndef _TABELA_H_
#define _TABELA_H_

#include <stdio.h>
#include "valor_lexico.h"

typedef struct posicao posicao_t;
struct posicao {
	int l;
	int c;
};

enum naturezaSimbolo { SYM_UNKNOWN, SYM_LITERAL, SYM_VARIAVEL, SYM_ARRANJO, SYM_FUNCAO };

enum tipoSimbolo { TYPE_UNDEFINED, TYPE_INT, TYPE_FLOAT, TYPE_CHAR, TYPE_BOOL };

typedef struct simbolo simbolo_t;
struct simbolo {
	posicao_t pos;
	int natureza;
	int tipo;
	int tamanhoB;
	valor_lexico* valor;
	// ... outros argumentos/função; dimensões/arranjo
};

typedef struct tabela_simbolo tabela_t;
struct tabela_simbolo {
	int count_symbols;
	simbolo_t** list;
};

typedef struct pilha pilha_t;
struct pilha {
	int count;
	tabela_t** tabelas;
};

simbolo_t* create_symbol(int lineno);
void destroy_symbol(simbolo_t* symbol);
// talvez uma funcão pra cada natureza de simbolo tipo "create_function_symbol" "create_array_symbol"...
void print_symbol(simbolo_t* symbol);

tabela_t* create_symbol_table();
void insert_symbol(tabela_t* table, char* key, simbolo_t* symbol);
simbolo_t* get_symbol(tabela_t* table, char* key);
void destroy_table(tabela_t* table);
void print_table(tabela_t* table);

pilha_t* create_pilha();
void push_table(pilha_t* pilha, tabela_t* table);
tabela_t* pop_table(pilha_t* pilha);
void destroy_pilha(pilha_t* pilha);

#endif //_TABELA_H_
