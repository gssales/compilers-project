#ifndef _ARVORE_H_
#define _ARVORE_H_

#include "utils.h"

typedef struct node node_t;
struct node {
  	int flag;
	char *label;
	valor_lexico *value;
	int count_children;
	node_t **children;
};

node_t* create_leaf(char* label, valor_lexico *value);
node_t* create_node(char* label);
node_t* asList_getLeaf(node_t *list);
node_t* getLastChildOfSameLabel(node_t *list);
node_t* getLastOf(node_t *list, int tipo);
void add_child(node_t *node, node_t *child); // adiciona um nodo no final da lista
void unshift_child(node_t *node, node_t *child); // adiciona um nodo no inicio da lista
void print_debug(node_t *tree);
/* função usada na main */
void libera(void* root);
/* função usada na main */
void exporta(void* root);
void exportTree(node_t *root, node_t *parent, FILE* e3csv);
int isLiteral(node_t* node);
int isNullNode(node_t* node);
#endif //_ARVORE_H_