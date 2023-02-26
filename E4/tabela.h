#ifndef _TABELA_H_
#define _TABELA_H_

#include <stdio.h>
#include "valor_lexico.h"
#include "stringstack.h"

#define ERR_UNDECLARED 10 //2.2
#define ERR_DECLARED 11 //2.2
#define ERR_VARIABLE 20 //2.3
#define ERR_ARRAY 21 //2.3
#define ERR_FUNCTION 22 //2.3
#define ERR_CHAR_TO_INT 31 //2.4
#define ERR_CHAR_TO_FLOAT 32 //2.4
#define ERR_CHAR_TO_BOOL 33 //2.4
#define ERR_CHAR_VECTOR 34 //2.4
#define ERR_X_TO_CHAR 35 //2.4

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

#define HASH_TABLE_SIZE 64

#define NOT_INSERTED -3
#define KEY_ALREADY_INSERTED -2
#define HASH_TABLE_FULL -1

typedef struct par_insercao par_insercao_t;
struct par_insercao {
	char* key;
	simbolo_t* symbol;
};

typedef struct tabela_simbolo tabela_t;
struct tabela_simbolo {
	int count_symbols;
	int* hashes;
	int size;
	par_insercao_t** list;
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
int insert_symbol(tabela_t* table, char* key, simbolo_t* symbol);
par_insercao_t* get_symbol(tabela_t* table, char* key);
void destroy_table(tabela_t* table);
void print_hash(tabela_t* table);
void print_table(tabela_t* table);

par_insercao_t* get_symbol_pilha(int lineno, pilha_t* pilha_tabela, char* key);
void print_pilha(pilha_t* pilha);

pilha_t* create_pilha();
void push_table(pilha_t* pilha, tabela_t* table);
tabela_t* pop_table(pilha_t* pilha);
void destroy_pilha(pilha_t* pilha);

void add_tipos_pilha_str(struct strpilha_t *pilha_str, tabela_t* table, int tipo);
int tktype_to_type(int tk_type);

char* natureza_simbolo_to_string(int naturezaSimbolo);
char* tipo_simbolo_to_string(int tipoSimbolo);

void erro_semantico(int erro, int lineno, char* key, simbolo_t* symbol);

#endif //_TABELA_H_
