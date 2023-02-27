#ifndef _STRINGSTACK_H_
#define _STRINGSTACK_H_

#include <stdlib.h>
#include <string.h>

/**
 * Type for individual stack entry
 */
struct strpilha_entry {
  char *data;
  struct strpilha_entry *next;
};

/**
 * Type for stack instance
 */
struct strpilha_t
{
  struct strpilha_entry *head;
  size_t stackSize;
};
struct strpilha_t *create_strpilha(void);
char *copyStr(char *str);
char* top_strpilha(struct strpilha_t *strpilha);
void pop_strpilha(struct strpilha_t *strpilha);
void push_strpilha(struct strpilha_t *strpilha, char *value);
void print_pilha_str(struct strpilha_t *strpilha);
void destroy_strpilha(struct strpilha_t *strpilha);

#endif //_STRINGSTACK_H_
