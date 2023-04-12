#include <stdlib.h>
#include <string.h>
#include "tabela.h"
#include "lexvalue.h"
#include "parser.tab.h"

void print_node(node_t *node) {
  printf(":%s type %d", node->label, node->sym_type);
  if (node->value != NULL) {
    printf(" -> ");
    print_tk_value(node->value);
  }
}

node_t* create_node(char* label) {
	node_t *node = malloc(sizeof(node_t));
	if (node != NULL) {
		node->label = strdup(label);
    node->value = NULL;
    node->count_children = 0;
    node->children = NULL;
    node->ast_type = AST_NONE;
    node->sym_type = SYM_UNKNOWN;
    node->code = NULL;
    node->count_tmpList = 0;
    node->tmpList = NULL;
    node->tmp = 0;
    node->tl = 0;
    node->fl = 0;
	}
	return node;
}

node_t* create_leaf(char* label, lexvalue_t *value) {
	node_t *node = create_node(label);
	if (node != NULL) {
    node->value = value;
	}
	return node;
}

node_t* get_last_of(node_t *list, ast_type_t ast_type) {
  if (list->ast_type == ast_type) {
    for (int i = 0; i < list->count_children; i++) {
      node_t* last = get_last_of(list->children[i], ast_type);
      if (last != NULL)
        return last;
    }
    return list;
  }
  return NULL;
}

list_node_t* _get_all_of(list_node_t* r, node_t *list, char* label) {
  if (r != NULL) {
    if (strcmp(list->label, label) == 0) {
      r->count = r->count + 1;
      r->nodes = realloc(r->nodes, r->count * sizeof(node_t*));
      r->nodes[r->count-1] = list;
    }

    for (int i = 0; i < list->count_children; i++)
      r = _get_all_of(r, list->children[i], label);
  }
  return r;
}

list_node_t* get_all_of(node_t *list, char* label) {
  list_node_t* results = malloc(sizeof(list_node_t));
  if (results != NULL) {
    results->count = 0;
    results->nodes = malloc(0);
    results = _get_all_of(results, list, label);
  }
  return results;
}

void destroy_list_node(list_node_t* r) {
  if (r != NULL) {
    free(r->nodes);
    free(r);
  }
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
    for (int d = 0; d < depth; d++) printf("| ");
    print_node(tree);
    print_program(tree->code);
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
    if (node->code)
      destroy_iloc_program(node->code);
    if (node->tmpList)
      free(node->tmpList);
		free(node);
    root = NULL;
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
