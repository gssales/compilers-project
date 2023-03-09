#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stringstack.h"

struct strpilha_t* create_strpilha(void)
{
  struct strpilha_t *stack = malloc(sizeof *stack);
  if (stack)
  {
    stack->head = NULL;
    stack->stackSize = 0;
  }
  return stack;
}

char* copyStr(char *str)
{
  char *tmp = malloc(strlen(str) + 1);
  if (tmp)
    strcpy(tmp, str);
  return tmp;
}

void push_strpilha(struct strpilha_t *strpilha, char *value)
{
  struct strpilha_entry *entry = malloc(sizeof *entry); 
  if (entry)
  {
    entry->data = copyStr(value);
    entry->next = strpilha->head;
    strpilha->head = entry;
    strpilha->stackSize++;
  }
}

char* top_strpilha(struct strpilha_t *strpilha)
{
  if (strpilha && strpilha->head)
    return strpilha->head->data;
  else
    return NULL;
}

void pop_strpilha(struct strpilha_t *strpilha)
{
  if (strpilha->head != NULL)
  {
    struct strpilha_entry *tmp = strpilha->head;
    strpilha->head = strpilha->head->next;
    free(tmp->data);
    free(tmp);
    strpilha->stackSize--;
  }
}

void print_pilha_str(struct strpilha_t *strpilha)
{
    printf("\nPilha_str: %s",strpilha->head->data);
    struct strpilha_entry *tmp = strpilha->head->next;
    while(tmp != NULL) {
        printf(", %s", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

void clear(struct strpilha_t *strpilha)
{
  while (strpilha->head != NULL)
  pop_strpilha(strpilha);
}

void free_pilha_str(struct strpilha_t **strpilha)
{
  clear(*strpilha);
  free(*strpilha);
  *strpilha = NULL;
}

void destroy_strpilha(struct strpilha_t *strpilha) {
  while (strpilha->head != NULL) {
    pop_strpilha(strpilha);
  }
  free(strpilha);
}
