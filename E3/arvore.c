#include <stdlib.h>
#include <string.h>
#include "arvore.h"

node_t* create_leaf(char* label, valor_lexico *value) {
	node_t *node = NULL;
	node = malloc(sizeof(node_t));
	if (node != NULL) {
		node->label = strdup(label);
		node->value = value;
    node->count_children = 0;
    node->children = NULL;
	}
	return node;
}

node_t* create_node(char* label) {
	node_t *node = NULL;
	node = malloc(sizeof(node_t));
	if (node != NULL) {
		node->label = strdup(label);
    node->count_children = 0;
    node->children = NULL;
	}
	return node;
}

void add_child(node_t *node, node_t *child) {
  if (node != NULL && child != NULL){
    node->count_children++;
    node->children = realloc(node->children, node->count_children * sizeof(node_t*));
    node->children[node->count_children-1] = child;
  }
}

void print_node(node_t *node) {
  printf(":%s ", node->label);
}

void _print_tree(node_t *tree, int depth) {
  if (tree != NULL) {
    for (int d = 0; d < depth;d++) printf("| ");
    print_node(tree);
    for (int i = 0; i < tree->count_children; i++) {
      _print_tree(tree->children[i], depth + 1);
    }
    printf("\n");    
  }
}
void print_tree(node_t *tree) {
  _print_tree(tree, 0);
}

void libera(void* root) {
	node_t* node = (node_t *)root;
	if (node != NULL) {
		for (int i = 0; i < node->count_children; i++)
			libera(node->children[i]);
		free(node->label);
		free(node->value);
		free(node->children);
		free(node);
	}
}