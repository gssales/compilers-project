#ifndef _STRINGSTACK_H_
#define _STRINGSTACK_H_

#include <stdlib.h>
#include <string.h>

/**
 * Type for individual stack entry
 */
typedef struct strstack_entry strstack_entry_t;
struct strstack_entry {
  char *data;
  strstack_entry_t *next;
};

/**
 * Type for stack instance
 */
typedef struct strstack
{
  strstack_entry_t *head;
  size_t stack_size;
} strstack_t;

strstack_t *create_strstack(void);
char* copy_str(char *str);
char* top_strstack(strstack_t *stack);
void pop_strstack(strstack_t *stack);
void push_strstack(strstack_t *stack, char *value);
void print_strstack(strstack_t *stack);
void destroy_strstack(strstack_t *stack);

#endif //_STRINGSTACK_H_
