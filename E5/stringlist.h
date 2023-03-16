#ifndef _STRINGLIST_H_
#define _STRINGLIST_H_

#include <stdlib.h>
#include <string.h>

typedef struct _strlist_node strlist_node_t;
typedef struct _strlist_node {
  char *str;
  strlist_node_t* next;
} strlist_node_t;

typedef struct _strlist strlist_t;
typedef struct _strlist {
  strlist_node_t* head;
  strlist_node_t* tail;
  int size;
} strlist_t;

strlist_t* create_strlist();

void add_strlist(strlist_t* strlist, char* str);

void print_strlist(strlist_t* strlist);

void clear_strlist(strlist_t* strlist);

char* copy_str(char *str);

#endif //_STRINGLIST_H_
