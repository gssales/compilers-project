#include <string.h>
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

u_int64_t hash_function32(char* str) {
  u_int64_t hash = 2166136261UL;
  for (const char* s = str; *s; s++) {
    hash ^= (u_int64_t) (unsigned char) (*s);
    hash *= 16777619UL;
  }
  return hash;
}

tabela_t* create_symbol_table() {
  tabela_t* t = malloc(sizeof(tabela_t));
  if (t != NULL) {
    t->count_symbols = 0;
    t->size = HASH_TABLE_SIZE;
    t->hashes = malloc(t->size * sizeof(u_int64_t));
    for (int i = 0; i < t->size; i++)
      t->hashes[i] = -1;
    t->list = NULL;
  }
  return t;
}

int get_free_index(tabela_t* table, char* key) {
  int pseudoindex = HASH_TABLE_FULL;
  if (table != NULL && key != NULL && *key) {
    int k = (int)(hash_function32(key) % table->size);
    for (int j = 0, i = k; j < table->size; i = (k+j) % table->size, j++) {
      if (table->hashes[i] == -1) {
        pseudoindex = i;
        break;
      }
      else {
        par_insercao_t *p = table->list[table->hashes[i]];
        if (strcmp(p->key, key) == 0) {
          pseudoindex = KEY_ALREADY_INSERTED;
          break;
        }
      }
    }
  }
  return pseudoindex;
}

void resize_table(tabela_t* table) {
  if (table != NULL) {
    table->size = table->size + HASH_TABLE_SIZE;
    free(table->hashes);
    table->hashes = malloc(table->size * sizeof(int));
    for (int i = 0; i < table->size; i++)
      table->hashes[i] = -1;
    for (int i = 0; i < table->count_symbols; i++) {
      par_insercao_t *p = table->list[i];
      int pseudoindex = get_free_index(table, p->key);
      table->hashes[pseudoindex] = i;
    }
  }
}

int insert_symbol(tabela_t* table, char* key, simbolo_t* symbol) {
  int pseudoindex = NOT_INSERTED;
  if (table != NULL && key != NULL && *key && symbol != NULL) {
    pseudoindex = get_free_index(table, key);
    if (pseudoindex == HASH_TABLE_FULL) {
      resize_table(table);
      pseudoindex = get_free_index(table, key);
    }
    // printf("%d\n", table->size);
    printf("%d %s\n", pseudoindex, key);
    if (pseudoindex >= 0) { // KEY_ALREADY_INSERTED
      par_insercao_t *par = malloc(sizeof(par_insercao_t));
      par->key = strdup(key);
      par->symbol = symbol;
      table->count_symbols++;
      table->hashes[pseudoindex] = table->count_symbols-1;
      table->list = realloc(table->list, table->count_symbols * sizeof(simbolo_t*));
      table->list[table->count_symbols-1] = par;
    }
  }
  return pseudoindex;
}

par_insercao_t* get_symbol(tabela_t* table, char* key) {
  par_insercao_t* par = NULL;
  if (table != NULL && key != NULL && *key) {
    int i = (int)(hash_function32(key) % table->size);
    for (int j = 0; j < table->size; j++, i = (i+j) % table->size) {
      if (table->hashes[i] != -1) {
        par_insercao_t *p = table->list[table->hashes[i]];
        if (strcmp(p->key, key) == 0) {
          par = p;
          break;
        }
      }
    }
  }
  return par;
}

void destroy_table(tabela_t* table) {
  if (table != NULL) {
    while (table->count_symbols > 0) {
      par_insercao_t* p = table->list[table->count_symbols-1];
      if (p != NULL) {
        free(p->key);
        destroy_symbol(p->symbol);
      }
      table->count_symbols--;
    }
    free(table->list);
    free(table->hashes);
    free(table);
  }
}

void print_hash(tabela_t* table) {
  if (table != NULL) {
    printf("size: %d\n", table->size);
    for (int i = 0; i < table->size; i++) {
      printf("%d\n", table->hashes[i]);
    }
  }
}

void print_table(tabela_t* table) {
  if (table != NULL) {
    printf("count symbols: %d\n", table->count_symbols);
    for (int i = 0; i < table->count_symbols; i++) {
      par_insercao_t* p = table->list[i];
      print_symbol(p->symbol);
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

void erro_semantico(int erro, int lineno) {
  // TODO: terminar as mensagens de erro, adicionar mais informacoes (identificadores e sua natureza)
  switch (erro) {
    case ERR_UNDECLARED: //2.2
      printf("ERR_UNDECLARED NA LINHA %d = EXIT(%d)\n", lineno, ERR_UNDECLARED);
    case ERR_DECLARED: //2.2
    case ERR_VARIABLE: //2.3
    case ERR_ARRAY: //2.3
    case ERR_FUNCTION: //2.3
    case ERR_CHAR_TO_INT: //2.4
    case ERR_CHAR_TO_FLOAT: //2.4
    case ERR_CHAR_TO_BOOL: //2.4
    case ERR_CHAR_VECTOR: //2.4
    case ERR_X_TO_CHAR: //2.4
  }
  exit(erro);
}