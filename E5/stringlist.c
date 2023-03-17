#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stringlist.h"

strlist_t* create_strlist() {
  strlist_t* strlist = (strlist_t*) malloc(sizeof(strlist_t*));
  if (strlist != NULL) {
    strlist->head = NULL;
    strlist->tail = NULL;
    strlist->size = 0;
  }
  return strlist;
}

void add_strlist(strlist_t* strlist, char* str) {
  if (strlist != NULL && str != NULL) {
    strlist_node_t* node = (strlist_node_t*) malloc(sizeof(strlist_node_t*));
    if (node != NULL) {
      node->str = copy_str(str);
      node->next = NULL;
      if (strlist->head == NULL) {
        strlist->head = node;
      }
      else {
        strlist->tail->next = node;
      }
      strlist->tail = node;
      strlist->size++;
    }
  }
}

void print_strlist(strlist_t* strlist) {
  strlist_node_t* node = strlist->head;
  int i = 0;
  printf("\nstrlist:\n");
  while (node != NULL) {
    if (node->str != NULL) {
      printf("%d: %s\n",i,node->str);
      fflush(stdout);
    }
    i++;
    node = node->next;
  }
}

void clear_strlist(strlist_t* strlist) {
  strlist_node_t* node = strlist->head;
  while (node != NULL) {
    strlist->head = node->next;
    free(node->str);
    free(node);
    node = strlist->head;
  }
  strlist->size = 0;
}

char* copy_str(char *str)
{
  char *tmp = malloc(strlen(str) + 1);
  if (tmp)
    strcpy(tmp, str);
  return tmp;
}