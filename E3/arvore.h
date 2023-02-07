#ifndef _ARVORE_H_
#define _ARVORE_H_

#include "utils.h"

typedef struct node node_t;
struct node {
	char *label;
	valor_lexico *value;
	int count_children;
	node_t **children;
};

node_t* create_leaf(char* label, valor_lexico *value);
node_t* create_node(char* label);
void add_child(node_t *node, node_t *child);
/* função usada na main */
void libera(void* root);
/* função usada na main */
// void exporta(void* root);
#endif //_ARVORE_H_