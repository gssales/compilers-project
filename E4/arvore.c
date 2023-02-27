#include <stdlib.h>
#include <string.h>
#include "tabela.h"
#include "valor_lexico.h"
#include "parser.tab.h"

void print_node(node_t *node) {
  printf(":%s type %d", node->label, node->type);
  if (node->value != NULL) {
    printf(" -> ");
    print_tk_value(node->value);
  }
}

node_t* create_leaf(char* label, valor_lexico *value) {
	node_t *node = NULL;
	node = malloc(sizeof(node_t));
	if (node != NULL) {
    node->label = strdup(label);
    node->value = value;
    node->count_children = 0;
    node->children = NULL;
    node->flag = 0; // FUNCAO/COMANDO/EXPRESSAO
    node->type = TYPE_UNDEFINED;
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
    node->type = TYPE_UNDEFINED;
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

node_t* getLastOf(node_t *list, int tipo) {
  if (list->flag == tipo) {
    for (int i = 0; i < list->count_children; i++) {
      node_t* funct = getLastOf(list->children[i], tipo);
      if (funct != NULL)
        return funct;
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

void _print_debug(node_t *tree, int depth) {
  if (tree != NULL) {
    for (int d = 0; d < depth;d++) printf("| ");
    print_node(tree);
    printf("\n");
    for (int i = 0; i < tree->count_children; i++) {
      _print_debug(tree->children[i], depth + 1);
    }
  }
}

void print_debug(node_t *tree) {
  _print_debug(tree, 0);
}

void print_var_list(node_t *var_list) {
  if (var_list != NULL) {
    printf("\n");
    print_node(var_list);
    for (int i = 0; i < var_list->count_children; i++) {
      print_var_list(var_list->children[i]);
    }
  }
}

void libera(void* root) {
	node_t* node = (node_t *)root;
	if (node != NULL) {
		for (int i = 0; i < node->count_children; i++)
			libera(node->children[i]);
    if (node->value)
		  destroy_lexvalue(node->value);
		free(node->label);
		free(node->children);
		free(node);
	}
}

void print_entrega(node_t *root, node_t *parent)
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
				print_entrega(root->children[i], root);
			}
		}
	}
}

void exporta(void* root) {
	node_t* node = (node_t *)root;
	print_entrega(node, NULL);
}

int isLiteral(node_t* node)
{
	return (node->value != NULL);
}