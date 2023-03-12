#ifndef _TABELA_H_
#define _TABELA_H_

#include "lexvalue.h"
#include "stringstack.h"
#include "types.h"

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

typedef unsigned int u_int32_t;

typedef struct symbol {
	int lineno;
	symbol_nature_t sym_nature;
	symbol_type_t sym_type;
	int sizeB;
	lexvalue_t* value;
	// ... outros argumentos/função; dimensões/arranjo
} symbol_t;

#define HASH_TABLE_SIZE 64
#define NOT_INSERTED -3
#define KEY_ALREADY_INSERTED -2
#define HASH_TABLE_FULL -1

typedef struct insert_pair {
	char* key;
	symbol_t* symbol;
} insert_pair_t;

typedef struct tabela_simbolo {
	int is_global_scope;
	int count_symbols;
	int* hashes;
	int size;
	insert_pair_t** list;
} table_t;

typedef struct stack {
	int count;
	table_t** tables;
} stack_t;

symbol_t* create_symbol(int lineno);
symbol_t* create_symbol_id(int lineno, lexvalue_t* lexvalue, symbol_type_t type);
symbol_t* create_symbol_array(int lineno, lexvalue_t* lexvalue, int size);
symbol_t* create_symbol_function(int lineno, lexvalue_t* lexvalue, symbol_type_t type);
symbol_t* create_symbol_literal(int lineno, lexvalue_t* lexvalue, symbol_type_t type);
void destroy_symbol(symbol_t* symbol);
// talvez uma funcão pra cada natureza de simbolo tipo "create_function_symbol" "create_array_symbol"...
void print_symbol(symbol_t* symbol);

table_t* create_symbol_table(int is_global_scope);
int insert_symbol(table_t* table, char* key, symbol_t* symbol);
int is_inserted(int result);
insert_pair_t* get_symbol(table_t* table, char* key);
void destroy_table(table_t* table);

void print_hash(table_t* table);
void print_table(table_t* table);

insert_pair_t* get_symbol_stack(int lineno, stack_t* table_stack, char* key);
void print_pilha(stack_t* pilha);

stack_t* create_stack();
void push_table(stack_t* stack, table_t* table);
int push_symbol_into_table(stack_t* stack, char* key, symbol_t* symbol);
table_t* pop_table(stack_t* stack);
table_t* get_table(stack_t* stack, int index);
void destroy_stack(stack_t* stack);

void add_types_to_strstack(strstack_t *strstack, table_t* table, int tk_type);
int tktype_to_type(int tk_type);
int calculate_size(symbol_t* s, symbol_type_t type);

void check_declared(int lineno, stack_t* table_stack, char* key);
void check_correct_use(int lineno, symbol_t *s, symbol_nature_t sym_nature);

void semantic_error(int erro, int lineno, char* key, symbol_t* symbol);

#endif //_TABELA_H_
