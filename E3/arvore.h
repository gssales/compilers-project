#ifndef _ARVORE_H_
#define _ARVORE_H_

#include "main.h"

typedef struct node node_t;
struct node {
	char *label;
	value_t *value;
	int count_children;
	node_t **children;
};

node_t* create_leaf(char* label, value_t *value);
node_t* create_node(char* label);
void add_child(node_t *node, node_t *child);
/* função usada na main */
void libera(void* root);
/* função usada na main */
// void exporta(void* root);
#endif //_ARVORE_H_