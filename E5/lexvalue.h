#ifndef LEXVALUE_H
#define LEXVALUE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef union tk_value {
  int i;
  float f;
  char c;
  char* s;
} tk_value_t;

typedef struct lexvalue {
  int line_number;
  int tk_type;
  char *lexema;
  tk_value_t tk_value;
} lexvalue_t;

lexvalue_t* create_lexvalue(int lineno, int token, char* lexema);
lexvalue_t* copy_lexvalue(lexvalue_t* lexvalue);
void destroy_lexvalue(lexvalue_t* lexvalue);

void print_lexvalue(lexvalue_t* lexvalue);
void print_tk_value(lexvalue_t* lexvalue);

#endif