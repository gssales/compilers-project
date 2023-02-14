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

simbolo_t* create_symbol(int lineno);
void destroy_symbol(simbolo_t* symbol);
// talvez uma funcão pra cada natureza de simbolo tipo "create_function_symbol" "create_array_symbol"...
void print_symbol(simbolo_t* symbol);

tabela_t* create_symbol_table();
void symbol_table_push(tabela_t* table, simbolo_t* symbol);
simbolo_t* symbol_table_pop(tabela_t* table);
// uma funcao para buscar e retornar o ptr de um simbolo na tabela
void destroy_table(tabela_t* table);
void print_table(tabela_t* table);

#endif //_TABELA_H_