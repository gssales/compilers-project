#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stringstack.h"

strstack_t *create_strstack(void)
{
  strstack_t *stack = malloc(sizeof(strstack_t));
  if (stack)
  {
    stack->head = NULL;
    stack->stack_size = 0;
  }
  return stack;
}

char* copy_str(char *str)
{
  char *tmp = malloc(strlen(str) + 1);
  if (tmp)
    strcpy(tmp, str);
  return tmp;
}

char* top_strstack(strstack_t *stack)
{
  if (stack && stack->head)
    return stack->head->data;
  else
    return NULL;
}

void pop_strstack(strstack_t *stack)
{
  if (stack && stack->head != NULL)
  {
    strstack_entry_t *tmp = stack->head;
    stack->head = stack->head->next;
    free(tmp->data);
    free(tmp);
    stack->stack_size--;
  }
}

void push_strstack(strstack_t *stack, char *value)
{
  if (stack) {
    strstack_entry_t *entry = malloc(sizeof(strstack_entry_t)); 
    if (entry)
    {
      entry->data = copy_str(value);
      entry->next = stack->head;
      stack->head = entry;
      stack->stack_size++;
    }
  }
}

void print_strstack(strstack_t *stack)
{
  if (stack) {
    printf("\nPilha_str: %s",stack->head->data);
    strstack_entry_t *tmp = stack->head->next;
    while(tmp != NULL) {
        printf(", %s", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
  }
}

void destroy_strstack(strstack_t *stack) {
  if (stack) {
    while (stack->head != NULL) {
      pop_strstack(stack);
    }
    free(stack);
    stack = NULL;
  }
}
