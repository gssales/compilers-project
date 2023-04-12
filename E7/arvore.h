#ifndef _ARVORE_H_
#define _ARVORE_H_

#include "types.h"
#include "lexvalue.h"
#include "iloc.h"

typedef struct node node_t;
struct node {
  ast_type_t ast_type;
	symbol_type_t sym_type;
	char *label;
	lexvalue_t *value;
	int count_children;
	node_t **children;

	// campos aux. geracao de codigo
	int count_tmpList;
	int *tmpList;
	int tmp;
	int tl;
	int fl;
	iloc_program_t *code;
};

typedef struct list_of_nodes {
	int count;
	node_t** nodes;
} list_node_t;

node_t* create_node(char* label);
node_t* create_leaf(char* label, lexvalue_t *value);

node_t* get_last_of(node_t *list, ast_type_t ast_type);
list_node_t* get_all_of(node_t *list, char* label);
void destroy_list_node(list_node_t* r);

void add_child(node_t *node, node_t *child); // adiciona um nodo no final da lista
void unshift_child(node_t *node, node_t *child); // adiciona um nodo no inicio da lista

/* função usada na main */
void libera(void* root);

/* função usada na main */
void exporta(void* root);
void exportTree(node_t *root, node_t *parent, FILE* e3csv);

void print_debug(node_t *tree);
void print_var_list(node_t *var_list);

#endif //_ARVORE_H_