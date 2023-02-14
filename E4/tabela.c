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
/* implementar tabela hash */
  tabela_t* t;
  t = malloc(sizeof(tabela_t));
  if (t != NULL) {
    t->count_symbols = 0;
    t->list = NULL;
  }
  return t;
}

void insert_symbol(tabela_t* table, char* key, simbolo_t* symbol) { // usar chave
/* implementar tabela hash */
  if (table != NULL && symbol != NULL) {
    table->count_symbols++;
    table->list = realloc(table->list, table->count_symbols * sizeof(simbolo_t*));
    table->list[table->count_symbols-1] = symbol;
  }
}

simbolo_t* get_symbol(tabela_t* table, char* key) {
/* implementar tabela hash */
  return NULL; // usar chave;
}

void destroy_table(tabela_t* table) {
  if (table != NULL) {
/* implementar tabela hash */
    //while (table->count_symbols > 0) {
      //simbolo_t* s = symbol_table_pop(table);
      //if (s != NULL) {
      //  destroy_symbol(s);
      //}
    //}
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

pilha_t* create_pilha() {
  pilha_t* p;
  p = malloc(sizeof(pilha_t));
  if (p != NULL) {
    p->count = 0;
    p->tabelas = NULL;
  }
  return p;  
}

void push_table(pilha_t* pilha, tabela_t* table) {
  if (pilha != NULL && table != NULL) {
    pilha->count++;
    pilha->tabelas = realloc(pilha->tabelas, pilha->count * sizeof(tabela_t));
    pilha->tabelas[pilha->count-1] = table;
  }
}

tabela_t* pop_table(pilha_t* pilha) {
  tabela_t* t = NULL;
  if (pilha != NULL && pilha->count > 0) {
    pilha->count--;
    t = pilha->tabelas[pilha->count];
    pilha->tabelas = realloc(pilha->tabelas, pilha->count * sizeof(tabela_t));
  }
  return t;
}

void destroy_pilha(pilha_t* pilha) {
  if (pilha != NULL) {
    while (pilha->count > 0) {
      tabela_t* t = pop_table(pilha);
      if (t != NULL)
        destroy_table(t);
    }
    free(pilha);
  }
}






