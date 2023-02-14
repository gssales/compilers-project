#include "tabela.h"
#include "valor_lexico.h"

simbolo_t* create_symbol(int lineno) {
  simbolo_t* s = NULL;
  s = malloc(sizeof(simbolo_t));
  if (s != NULL) {
    s->pos.l = lineno;
    s->pos.c = -1;
    s->natureza = SYM_UNKNOWN;
    s->tipo = TYPE_UNDEFINED;
    s->tamanhoB = 0;
    s->valor = NULL;
  }
  return s;
}

void destroy_symbol(simbolo_t* symbol) {
  if (symbol != NULL) {
    if (symbol->valor)
      destroy_lexvalue(symbol->valor);
    free(symbol);
  }
}

void print_symbol(simbolo_t* symbol) {
  if (symbol != NULL) {
    printf("l:%d c:%d ", symbol->pos.l, symbol->pos.c);
    if (symbol->valor) {
      printf("lex: ");
      print_lexvalue(symbol->valor);
    }
    printf("\n");
  }
}

tabela_t* create_symbol_table() {
  tabela_t* t;
  t = malloc(sizeof(tabela_t));
  if (t != NULL) {
    t->count_symbols = 0;
    t->list = NULL;
  }
  return t;
}

void symbol_table_push(tabela_t* table, simbolo_t* symbol) {
  if (table != NULL && symbol != NULL) {
    table->count_symbols++;
    table->list = realloc(table->list, table->count_symbols * sizeof(simbolo_t*));
    table->list[table->count_symbols-1] = symbol;
  }
}

simbolo_t* symbol_table_pop(tabela_t* table) {
  simbolo_t* s = NULL;
  if (table != NULL) {
    table->count_symbols--;
    s = table->list[table->count_symbols];
    table->list = realloc(table->list, table->count_symbols * sizeof(simbolo_t*));
  }
  return s;
}

void destroy_table(tabela_t* table) {
  if (table != NULL) {
    while (table->count_symbols > 0) {
      simbolo_t* s = symbol_table_pop(table);
      if (s != NULL) {
        destroy_symbol(s);
      }
    }
    free(table);
  }
}

void print_table(tabela_t* table) {
  if (table != NULL) {
    printf("count symbols: %d\n", table->count_symbols);
    for (int i = 0; i < table->count_symbols; i++) {
      simbolo_t* s = table->list[i];
      print_symbol(s);
    }
  }
}
