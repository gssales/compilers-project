#include <string.h>
#include "tabela.h"
#include "parser.tab.h"

symbol_t* create_symbol(int lineno) {
  symbol_t* s = malloc(sizeof(symbol_t));
  if (s != NULL) {
    s->lineno = lineno;
    s->sym_nature = SYM_UNKNOWN;
    s->sym_type = TYPE_UNDEFINED;
    s->sizeB = 0;
    s->disp = 0;
    s->value = NULL;
    s->disp = 0;
    s->global = 0;
    s->label = -1;
  }
  return s;
}

symbol_t* create_symbol_id(int lineno, lexvalue_t* lexvalue, symbol_type_t type) {
  symbol_t* s = create_symbol(lineno);
  if (s != NULL) {
    s->sym_nature = SYM_VARIAVEL;
    s->value = copy_lexvalue(lexvalue);
    s->sym_type = type;
    s->sizeB = calculate_size(s, s->sym_type);
  }
  return s;
}

symbol_t* create_symbol_array(int lineno, lexvalue_t* lexvalue, int size) {
  symbol_t* s = create_symbol(lineno);
  if (s != NULL) {
    s->sym_nature = SYM_ARRANJO;
    s->value = copy_lexvalue(lexvalue);
    s->sizeB = size;
  }
  return s;
}

symbol_t* create_symbol_function(int lineno, lexvalue_t* lexvalue, symbol_type_t type) {
  symbol_t* s = create_symbol(lineno);
  if (s != NULL) {
    s->sym_nature = SYM_FUNCAO;
    s->value = copy_lexvalue(lexvalue);
    s->sym_type = type;
  }
  return s;
}

symbol_t* create_symbol_literal(int lineno, lexvalue_t* lexvalue, symbol_type_t type) {
  symbol_t* s = create_symbol(lineno);
  if (s != NULL) {
    s->sym_nature = SYM_LITERAL;
    s->value = copy_lexvalue(lexvalue);
    s->sym_type = type;
    s->sizeB = calculate_size(s, s->sym_type);
  }
  return s;
}

void destroy_symbol(symbol_t* symbol) {
  if (symbol != NULL) {
    if (symbol->value != NULL) {
      destroy_lexvalue(symbol->value);
      symbol->value = NULL;
    }
    free(symbol);
  }
}

void print_symbol(symbol_t* symbol) {
  if (symbol != NULL) {
    printf("l:%d\t", symbol->lineno);
    printf("nat: %s\ttipo: %s\ttam: %d\tdisp: %d\t",sym_nature_to_string(symbol->sym_nature),sym_type_to_string(symbol->sym_type),symbol->sizeB,symbol->disp);
    if (symbol->value) {
      printf("lex: ");
      print_lexvalue(symbol->value);
    }
    printf("\n");
  }
}

u_int32_t hash_function32(char* str) {
  u_int32_t hash = 2166136261UL;
  for (const char* s = str; *s; s++) {
    hash ^= (u_int32_t) (unsigned char) (*s);
    hash *= 16777619UL;
  }
  return hash;
}

table_t* create_symbol_table(int is_global_scope, int is_main_function, int initial_disp) {
  table_t* t = malloc(sizeof(table_t));
  if (t != NULL) {
    t->is_global_scope = is_global_scope;
    t->is_main_function = is_main_function;
    t->end_label = -1;
    t->disp = initial_disp;
    t->count_symbols = 0;
    t->size = HASH_TABLE_SIZE;
    t->hashes = malloc(t->size * sizeof(int));
    for (int i = 0; i < t->size; i++)
      t->hashes[i] = -1;
    t->list = NULL;
  }
  return t;
}

int get_free_index(table_t* table, char* key) {
  int pseudoindex = HASH_TABLE_FULL;
  if (table != NULL && key != NULL && *key) {
    int k = (int)(hash_function32(key) % table->size);
    for (int j = 0, i = k; j < table->size; i = (k+j) % table->size, j++) {
      if (table->hashes[i] == -1) {
        pseudoindex = i;
        break;
      }
      else {
        insert_pair_t *p = table->list[table->hashes[i]];
        if (strcmp(p->key, key) == 0) {
          pseudoindex = KEY_ALREADY_INSERTED;
          break;
        }
      }
    }
  }
  return pseudoindex;
}

void resize_table(table_t* table) {
  if (table != NULL) {
    table->size = table->size + HASH_TABLE_SIZE;
    free(table->hashes);
    table->hashes = malloc(table->size * sizeof(int));
    for (int i = 0; i < table->size; i++)
      table->hashes[i] = -1;
    for (int i = 0; i < table->count_symbols; i++) {
      insert_pair_t *p = table->list[i];
      int pseudoindex = get_free_index(table, p->key);
      table->hashes[pseudoindex] = i;
    }
  }
}

int insert_symbol(table_t* table, char* key, symbol_t* symbol) {
  int pseudoindex = NOT_INSERTED;
  if (table != NULL && key != NULL && *key && symbol != NULL) {
    pseudoindex = get_free_index(table, key);
    if (pseudoindex == HASH_TABLE_FULL) {
      resize_table(table);
      pseudoindex = get_free_index(table, key);
    }
    if (pseudoindex >= 0) { // KEY_ALREADY_INSERTED
      insert_pair_t *par = malloc(sizeof(insert_pair_t));
      par->key = strdup(key);
      symbol->global = table->is_global_scope;
      if (symbol->sym_nature == SYM_VARIAVEL || symbol->sym_nature == SYM_ARRANJO) {
        symbol->disp = table->disp;
        table->disp += symbol->sizeB;
      }
      par->symbol = symbol;
      table->count_symbols++;
      table->hashes[pseudoindex] = table->count_symbols-1;
      table->list = realloc(table->list, table->count_symbols * sizeof(symbol_t*));
      table->list[table->count_symbols-1] = par;
    }
  }
  return pseudoindex;
}

int push_symbol_into_table(stack_t* stack, char* key, symbol_t* symbol) {
  int pseudoindex = NOT_INSERTED;
  if (stack != NULL && stack->count > 0 && key != NULL && *key && symbol != NULL) {
    if (symbol->sym_nature != SYM_LITERAL && symbol->value != NULL) {
      check_declared(symbol->value->line_number, stack, key);
    }
    table_t* top_table = get_table(stack, stack->count-1);
    pseudoindex = insert_symbol(top_table, key, symbol);
  }
  return pseudoindex;
}

int is_inserted(int result) {
  return result != NOT_INSERTED && result != KEY_ALREADY_INSERTED && result != HASH_TABLE_FULL;
}

insert_pair_t* get_symbol(table_t* table, char* key) {
  insert_pair_t* par = NULL;
  if (table != NULL && key != NULL) {
    int i = (int)(hash_function32(key) % table->size);
    for (int j = 0; j < table->size; j++, i = (i+j) % table->size) {
      if (table->hashes[i] != -1) {
        insert_pair_t *p = table->list[table->hashes[i]];
        if (strcmp(p->key, key) == 0) {
          par = p;
          break;
        }
      }
    }
  }
  return par;
}

insert_pair_t* get_symbol_stack(int lineno, stack_t* table_stack, char* key) {
  insert_pair_t* pair = NULL;
  for (int i = table_stack->count-1; i >= 0; i--) {
    pair = get_symbol(table_stack->tables[i], key);
    if (pair != NULL) {
      break;
    }
  }
  // ERR_UNDECLARED: NAO ENCONTROU SIMBOLO EM NENHUM ESCOPO
  if (pair == NULL) { 
    semantic_error(ERR_UNDECLARED, lineno, key, NULL);
  }
  return pair;
}

void check_declared(int lineno, stack_t* table_stack, char* key) {
  insert_pair_t* par = NULL;
  table_t* t = get_table(table_stack, table_stack->count-1);
  par = get_symbol(t, key);
  // ERR_DECLARED: JA EXISTE SIMBOLO COM ESSE IDENTIFICADOR
  if (par != NULL) { 
    symbol_t *s = par->symbol;
    if (s->sym_nature != SYM_LITERAL) {
      semantic_error(ERR_DECLARED, s->lineno, key, s);
    }
  }
}

void check_correct_use(int lineno, symbol_t *s, symbol_nature_t sym_nature) {
  // ERR_VARIAVEL, ERR_ARRAY, ERR_FUNCTION
  if (s && s->sym_nature != sym_nature) {
    switch(s->sym_nature) {
      case SYM_VARIAVEL:
        semantic_error(ERR_VARIABLE, lineno, s->value->tk_value.s, s);
        break;
      case SYM_ARRANJO:
        semantic_error(ERR_ARRAY, lineno, s->value->tk_value.s, s);
        break;
      case SYM_FUNCAO:
        semantic_error(ERR_FUNCTION, lineno, s->value->tk_value.s, s);
        break;
    }
  }
}

void destroy_table(table_t* table) {
  if (table != NULL) {
    while (table->count_symbols > 0) {
      insert_pair_t* p = table->list[table->count_symbols-1];
      if (p != NULL) {
        // if (p->symbol->sym_nature == SYM_LITERAL && p->symbol->sym_type == TYPE_BOOL) print_symbol(p->symbol);

        free(p->key);
        destroy_symbol(p->symbol);
      }
      free(p);
      table->count_symbols--;
    }
    free(table->list);
    free(table->hashes);
    free(table);
    table = NULL;
  }
}

void print_hash(table_t* table) {
  if (table != NULL) {
    printf("size: %d\n", table->size);
    for (int i = 0; i < table->size; i++) {
      printf("%d\n", table->hashes[i]);
    }
  }
}

void print_table(table_t* table) {
  if (table != NULL) {
    printf("count symbols: %d\n", table->count_symbols);
    for (int i = 0; i < table->count_symbols; i++) {
      insert_pair_t* p = table->list[i];
      print_symbol(p->symbol);
    }
  }
}

void print_pilha(stack_t* stack) {
  if (stack != NULL) {
    printf("\n========== PILHA FINAL ==========\n");
    for (int i = 0; i < stack->count; i++) {
      printf("---> Escopo %d \n", i);
      print_table(stack->tables[i]);
    }
  }
}

stack_t* create_stack() {
  stack_t* p = malloc(sizeof(stack_t));
  if (p != NULL) {
    p->count = 0;
    p->tables = NULL;
  }
  return p;  
}

void push_table(stack_t* stack, table_t* table) {
  if (stack != NULL && table != NULL) {
    stack->count++;
    stack->tables = realloc(stack->tables, stack->count * sizeof(table_t));
    stack->tables[stack->count-1] = table;
  }
}

table_t* pop_table(stack_t* stack) {
  table_t* t = NULL;
  if (stack != NULL && stack->count > 0) {
    stack->count--;
    t = stack->tables[stack->count];
    stack->tables = realloc(stack->tables, stack->count * sizeof(table_t));
  }
  return t;
}

table_t* get_table(stack_t* stack, int index) {
  table_t* t = NULL;
  if (stack != NULL && index < stack->count) {
    t = stack->tables[index];
  }
  return t;
}

void destroy_stack(stack_t* stack) {
  if (stack != NULL) {
    while (stack->count > 0) {
      table_t* t = pop_table(stack);
      if (t != NULL)
        destroy_table(t);
    }
    free(stack);
    stack = NULL;
  }
}

void add_types_to_strlist(strlist_t *strlist, table_t* table, int tk_type) {
  if (strlist && table) {
    tk_type = tktype_to_type(tk_type);
    
    strlist_node_t* node = strlist->head;
    char *str = node->str;
    while (node != NULL) {
      symbol_t* s = get_symbol(table, str)->symbol;

      // ERR_CHAR_VECTOR
      if (tk_type == TYPE_CHAR && s->sym_nature == SYM_ARRANJO) {
        semantic_error(ERR_CHAR_VECTOR, s->lineno, str, s);
      }
      // adiciona tamanho
      int tam = calculate_size(s, tk_type);
      s->sizeB = tam;

      // adiciona deslocamento
      s->disp = table->disp;
      table->disp += s->sizeB;

      // adiciona tipo
      s->sym_type = tk_type;

      node = node->next;
      if (node != NULL) {
        str = node->str;
      }
    }
    //clear_strlist(strlist);
  }
}

int calculate_size(symbol_t* s, symbol_type_t type) {
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
    if (s->sym_nature == SYM_ARRANJO) {
      // tam_base = tam_base * dimensoes
      tam_base = tam_base * s->sizeB;
    }

    return tam_base;
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

void semantic_error(int erro, int lineno, char* key, symbol_t* symbol) {
  switch (erro) {
    case ERR_UNDECLARED: //2.2
      printf("Linha %d - ERR_UNDECLARED: Identificador '%s' não declarado\n", lineno, key);
      break;
    case ERR_DECLARED: //2.2
      printf("Linha %d - ERR_DECLARED: Dupla declaração do identificador '%s'\n", lineno, symbol->value->tk_value.s);
      break;
    case ERR_VARIABLE: //2.3
      printf("Linha %d - ERR_VARIABLE: Identificador de variável '%s' sendo usado como arranjo ou função\n", lineno, symbol->value->tk_value.s);
      break;
    case ERR_ARRAY: //2.3
      printf("Linha %d - ERR_ARRAY: Identificador de arranjo '%s' sendo usado como variável ou função\n", lineno, symbol->value->tk_value.s);
      break;
    case ERR_FUNCTION: //2.3
      printf("Linha %d - ERR_FUNCTION: Identificador de função '%s' sendo usado como variável ou arranjo\n", lineno, symbol->value->tk_value.s);
      break;
    case ERR_CHAR_TO_INT: //2.4
      printf("Linha %d - ERR_CHAR_TO_INT: Tentativa de coerção da variável '%s' do tipo char para o tipo int\n", lineno, symbol->value->tk_value.s);
      break;
    case ERR_CHAR_TO_FLOAT: //2.4
      printf("Linha %d - ERR_CHAR_TO_FLOAT: Tentativa de coerção da variável '%s' do tipo char para o tipo float\n", lineno, symbol->value->tk_value.s);
      break;
    case ERR_CHAR_TO_BOOL: //2.4
      printf("Linha %d - ERR_CHAR_TO_BOOL: Tentativa de coerção da variável '%s' do tipo char para o tipo bool\n", lineno, symbol->value->tk_value.s);
      break;
    case ERR_CHAR_VECTOR: //2.4
      printf("Linha %d - ERR_CHAR_VECTOR: Arranjo '%s' declarado com o tipo char\n", lineno, symbol->value->tk_value.s);
      break;
    case ERR_X_TO_CHAR: //2.4
      printf("Linha %d - ERR_X_TO_CHAR: Tentativa de coerção da variável '%s' do tipo int/float/bool para o tipo char\n", lineno, symbol->value->tk_value.s);
      break;
  }
  exit(erro);
}