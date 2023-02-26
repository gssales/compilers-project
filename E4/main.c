/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>

#include "tabela.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void* pilha_tabelas;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse();
  //exporta(arvore);
  libera(arvore);
  //printf("\n============= TABELA =============\n");
  //pilha_t *pilha = pilha_tabelas;
  //print_table(pilha->tabelas[0]);
  arvore = NULL;
  yylex_destroy();
  return ret;
  
  /*
  // testes basicos pilhas e tabelas
  pilha_t *pilha_tabelas = create_pilha();
  tabela_t *t = create_symbol_table();
  tabela_t *t2 = create_symbol_table();
  push_table(pilha_tabelas, t2);
  pop_table(pilha_tabelas);
  push_table(pilha_tabelas, t);

  valor_lexico *v1 = create_lexvalue(-1,281,"lexema_id_v1_int");
  simbolo_t *s1 = create_symbol(1, SYM_VARIAVEL, TYPE_INT,v1);

  valor_lexico *v2 = create_lexvalue(-1,281,"lexema_id_v2_char");
  simbolo_t *s2 = create_symbol(1, SYM_VARIAVEL, TYPE_CHAR,v2);
  
  insert_symbol(t,s1->valor->tk_value.s, s1);
  par_insercao_t *p1 = get_symbol(2, t, s1->valor->tk_value.s);
  print_symbol(p1->symbol);

  // teste err_declared na declaracao
  insert_symbol(t,s2->valor->tk_value.s, s2);
  insert_symbol(t,s2->valor->tk_value.s, s2);
  par_insercao_t *p2 = get_symbol(3, t, s2->valor->tk_value.s);
  print_symbol(p2->symbol);

  // teste err_undeclared no uso
  par_insercao_t *p3 = get_symbol(5, t, "test_err_undeclared");

  print_table(t);*/
}