#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//enum tipoToken {RESERVADA, ESPECIAL, OPCOMPOSTO, IDENTIFICADOR, LITERAL};
//enum tipoLiteral {INT, CHAR, FLOAT, STRING, BOOL, NAOLITERAL};

typedef union TokenValue {
  int i;
  float f;
  char c;
  char* s; // usar string yytext caso nao seja literal?
} token_value_t;

typedef struct valor_lexico valor_lexico;
struct valor_lexico {
  int line_number;
  int tk_type;
  // char *str;
  token_value_t tk_value;
};

valor_lexico* create_lexvalue(int lineno, int token, char* lexema);

void print_lexvalue(valor_lexico* lexvalue);
void print_tk_value(valor_lexico* lexvalue);

#endif