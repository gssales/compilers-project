#include <string.h>
#include "tabela.h"
#include "parser.tab.h"

simbolo_t* create_symbol(int lineno) {
  simbolo_t* s = NULL;
  s = malloc(sizeof(simbolo_t));
  if (s != NULL) {
    s->pos.l = lineno;
    s->pos.c = -1; // se quisermos tentar: 'Print Column' @https://stackoverflow.com/questions/62115979/how-to-implement-better-error-messages-for-flex-bison
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
    printf("l:%d\tc:%d\t", symbol->pos.l, symbol->pos.c);
    if (symbol->valor) {
      printf("nat: %s\ttipo: %s\ttam: %d\t",natureza_simbolo_to_string(symbol->natureza),tipo_simbolo_to_string(symbol->tipo),symbol->tamanhoB);
      printf("lex: ");
      print_lexvalue(symbol->valor);
    }
    //printf("\n");
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
    printf("Insert_symbol: %d %s\n", pseudoindex, key);
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

par_insercao_t* get_symbol_pilha(int lineno, pilha_t* pilha_tabela, char* key) {
  par_insercao_t* par = NULL;
  for (int i = pilha_tabela->count-1; i >= 0; i--) {
    par = get_symbol(pilha_tabela->tabelas[i], key);
    if (par != NULL) {
      break;
    }
  }
  // ERR_UNDECLARED: NAO ENCONTROU SIMBOLO EM NENHUM ESCOPO
  if (par == NULL) { 
    erro_semantico(ERR_UNDECLARED, lineno, key, NULL);
  }
  return par;
}

void check_declared(int lineno, pilha_t* pilha_tabela, char* key) {
  par_insercao_t* par = NULL;
  for (int i = pilha_tabela->count; i >= 0; i--) {
    par = get_symbol(pilha_tabela->tabelas[i], key);
    // ERR_DECLARED: JA EXISTE SIMBOLO COM ESSE IDENTIFICADOR
    if (par != NULL) { 
      simbolo_t *s = par->symbol;
      if (s->natureza != SYM_LITERAL) {
        erro_semantico(ERR_DECLARED, s->pos.l, key, s);
      }
    }
  }
}

void check_correct_use(int lineno, simbolo_t *s, enum naturezaSimbolo nat) {
  // ERR_VARIAVEL, ERR_ARRAY, ERR_FUNCTION
  if (s->natureza != nat) {
    switch(s->natureza) {
      case SYM_VARIAVEL:
        erro_semantico(ERR_VARIABLE, lineno, s->valor->tk_value.s, s);
        break;
      case SYM_ARRANJO:
        erro_semantico(ERR_ARRAY, lineno, s->valor->tk_value.s, s);
        break;
      case SYM_FUNCAO:
        erro_semantico(ERR_FUNCTION, lineno, s->valor->tk_value.s, s);
        break;
    }
  }
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

void print_pilha(pilha_t* pilha) {
  if (pilha != NULL) {
    printf("\n========== PILHA FINAL ==========\n");
    for (int i = 0; i < pilha->count; i++) {
      printf("---> Escopo %d \n", i);
      print_table(pilha->tabelas[i]);
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
  // printa tabela antes de deletar (para debugar)
  printf("\n---> Escopo %d (Desempilhado)\n", pilha->count-1);
  print_table(pilha->tabelas[pilha->count-1]);

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

void add_tipos_pilha_str(struct strpilha_t *pilha_str, tabela_t* table, int tipo) {
  tipo = tktype_to_type(tipo);
  
  char *str;
  str = top_strpilha(pilha_str);
  //printf("\nAdd_tipos_pilha_str:\n");
  while (str != NULL) {
    //printf("- id = %s, tipo = %s\n",str,tipo_simbolo_to_string(tipo));
    simbolo_t* s = get_symbol(table, str)->symbol;

    // ERR_CHAR_VECTOR
    if (tipo == TYPE_CHAR && s->natureza == SYM_ARRANJO) {
      erro_semantico(ERR_CHAR_VECTOR, s->pos.l, str, s);
    }
    // adiciona tamanho
    calcula_tam(s, tipo);

    // adiciona tipo
    s->tipo = tipo;

    //printf("id %s recebeu tipo %s",par->symbol->valor->tk_value.s,tipo_simbolo_to_string(tipo));
    pop_strpilha(pilha_str);
    str = top_strpilha(pilha_str);
  }
  printf("\n");
}

void calcula_tam(simbolo_t* s, enum tipoSimbolo type) {
    int tam_base = 0;
    switch(type) {
      case TYPE_INT:
        tam_base = 4;
        break;
      case TYPE_BOOL:
        tam_base = 1;
        break;
      case TYPE_FLOAT:
        tam_base = 8;
        break;
      case TYPE_CHAR:
        tam_base = 1;
        break;
    }
    if (s->natureza == SYM_ARRANJO) {
      // tam_base = tam_base * dimensoes
      tam_base = tam_base * s->tamanhoB;
    }

    s->tamanhoB = tam_base;
}

int tktype_to_type(int tk_type) {
  switch(tk_type) {
    case TK_PR_INT:
      return TYPE_INT;
      break;
    case TK_PR_FLOAT:
      return TYPE_FLOAT;
      break;
    case TK_PR_BOOL:
      return TYPE_BOOL;
      break;
    case TK_PR_CHAR:
      return TYPE_CHAR;
      break;
  }
}

char* natureza_simbolo_to_string(int naturezaSimbolo) {
  switch(naturezaSimbolo) {
    case SYM_UNKNOWN:       return "SYM_UNKNOWN ";       break;
    case SYM_LITERAL:       return "SYM_LITERAL ";     break;
    case SYM_VARIAVEL:      return "SYM_VARIAVEL";     break;
    case SYM_ARRANJO:       return "SYM_ARRANJO ";      break;
    case SYM_FUNCAO:        return "SYM_FUNCAO  ";        break;
  }
}

char* tipo_simbolo_to_string(int tipoSimbolo) {
  switch(tipoSimbolo) {
    case TYPE_UNDEFINED:   return "TYPE_UNDEFINED";     break;
    case TYPE_INT:         return "TYPE_INT      ";           break;
    case TYPE_FLOAT:       return "TYPE_FLOAT    ";         break;
    case TYPE_CHAR:        return "TYPE_CHAR     ";          break;
    case TYPE_BOOL:        return "TYPE_BOOL     ";          break;
  }
}

void erro_semantico(int erro, int lineno, char* key, simbolo_t* symbol) {
  switch (erro) {
    case ERR_UNDECLARED: //2.2
      printf("Linha %d - ERR_UNDECLARED: Identificador '%s' não declarado\n", lineno, key);
      break;
    case ERR_DECLARED: //2.2
      printf("Linha %d - ERR_DECLARED: Dupla declaração do identificador '%s'\n", lineno, symbol->valor->tk_value.s);
      break;
    case ERR_VARIABLE: //2.3
      printf("Linha %d - ERR_VARIABLE: Identificador de variável '%s' sendo usado como arranjo ou função\n", lineno, symbol->valor->tk_value.s);
      break;
    case ERR_ARRAY: //2.3
      printf("Linha %d - ERR_ARRAY: Identificador de arranjo '%s' sendo usado como variável ou função\n", lineno, symbol->valor->tk_value.s);
      break;
    case ERR_FUNCTION: //2.3
      printf("Linha %d - ERR_FUNCTION: Identificador de função '%s' sendo usado como variável ou arranjo\n", lineno, symbol->valor->tk_value.s);
      break;
    case ERR_CHAR_TO_INT: //2.4
      printf("Linha %d - ERR_CHAR_TO_INT: Tentativa de coerção da variável '%s' do tipo char para o tipo int\n", lineno, symbol->valor->tk_value.s);
      break;
    case ERR_CHAR_TO_FLOAT: //2.4
      printf("Linha %d - ERR_CHAR_TO_FLOAT: Tentativa de coerção da variável '%s' do tipo char para o tipo float\n", lineno, symbol->valor->tk_value.s);
      break;
    case ERR_CHAR_TO_BOOL: //2.4
      printf("Linha %d - ERR_CHAR_TO_BOOL: Tentativa de coerção da variável '%s' do tipo char para o tipo bool\n", lineno, symbol->valor->tk_value.s);
      break;
    case ERR_CHAR_VECTOR: //2.4
      printf("Linha %d - ERR_CHAR_VECTOR: Arranjo '%s' declarado com o tipo char\n", lineno, symbol->valor->tk_value.s);
      break;
    case ERR_X_TO_CHAR: //2.4
      printf("Linha %d - ERR_X_TO_CHAR: Tentativa de coerção da variável '%s' do tipo int/float/bool para o tipo char\n", lineno, symbol->valor->tk_value.s);
      break;
  }
  exit(erro);
}