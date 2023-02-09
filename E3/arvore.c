#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "utils.h"
#include "parser.tab.h"

node_t* create_leaf(char* label, valor_lexico *value) {
	node_t *node = NULL;
	node = malloc(sizeof(node_t));
	if (node != NULL) {
		node->label = strdup(label);
		node->value = value;
    node->count_children = 0;
    node->children = NULL;
    node->flag = 0; // FUNCAO/COMANDO/EXPRESSAO
	}
	return node;
}

node_t* create_node(char* label) {
	node_t *node = NULL;
	node = malloc(sizeof(node_t));
	if (node != NULL) {
		node->label = strdup(label);
    node->value = NULL;
    node->count_children = 0;
    node->children = NULL;
    node->flag = 0;
	}
	return node;
}

node_t* asList_getLeaf(node_t *list) {
  if (list->count_children == 0)
    return list;
  if (list->count_children == 1)
    return asList_getLeaf(list->children[0]);
  else
    printf("ERROR node is not a list: %s\n", list->label);
}

node_t* getLastChildOfSameLabel(node_t *list) {
  if (list->count_children == 0)
    return list;
  else {
    for (int i = 0; i < list->count_children; i++) 
      if(strcmp(list->label, list->children[i]->label) == 0)
        return getLastChildOfSameLabel(list->children[i]);
    return list;
  }
}

node_t* getLastOf(node_t *list) {
  if (list->flag == FUNCAO) {
    for (int i = 0; i < list->count_children; i++) {
      node_t* funct = getLastOf(list->children[i]);
      if (funct != NULL)
        return funct;
    }
    return list;
  }
  if (list->flag == COMANDO) {
    for (int i = 0; i < list->count_children; i++) {
      node_t* cmd = getLastOf(list->children[i]);
      if (cmd != NULL)
        return cmd;
    }
    return list;
  }
  if (list->flag == EXPRESSAO) {
    for (int i = 0; i < list->count_children; i++) {
      node_t* expr = getLastOf(list->children[i]);
      if (expr != NULL)
        return expr;
    }
    return list;
  }
  return NULL;
}

void add_child(node_t *node, node_t *child) {
  if (node != NULL && child != NULL){
    node->count_children++;
    node->children = realloc(node->children, node->count_children * sizeof(node_t*));
    node->children[node->count_children-1] = child;
  }
}

void unshift_child(node_t *node, node_t *child) {
  if (node != NULL && child != NULL){
    node->count_children++;
    node->children = realloc(node->children, node->count_children * sizeof(node_t*));
    for (int i = node->count_children-1; i > 0; i--) 
      node->children[i] = node->children[i-1];
    node->children[0] = child;
  }
}

void print_node(node_t *node) {
  printf(":%s", node->label);
  if (node->value != NULL) {
    printf(" -> ");
    print_tk_value(node->value);
  }
}

void _print_tree(node_t *tree, int depth) {
  if (tree != NULL) {
    for (int d = 0; d < depth;d++) printf("| ");
    print_node(tree);
    printf("\n");
    for (int i = 0; i < tree->count_children; i++) {
      _print_tree(tree->children[i], depth + 1);
    }
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

void printTree(node_t *root, node_t *parent)
{
	if(root != NULL)
	{
		if(parent != NULL) { // imprime aresta pai-filho
			printf("%p, %p\n", parent, root);
			fflush(stdout);
		}

		printf("%p [label=\"%s\"];\n", root, root->label);
		fflush(stdout);

		if(root->count_children != 0) { // recursao
			for (int i = 0; i < root->count_children; i++) {
				printTree(root->children[i], root);
			}
		}
	}
}

void exporta(void* root) {
	//node_t* arvore = arvore; ONDE ACESSAR A RAIZ?
	
	node_t* node = (node_t *)root;
	printTree(node, NULL);
}

int isLiteral(node_t* node)
{
	return (node->value != NULL);
}