/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>

#include "tabela.h"
#include "iloc.h"
#include "arvore.h"
#include "cfg.h"
#include "asm.h"
//#include "parser.tab.h"
extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void* table_stack;
void* strstack;
void* strlist;
int rfp;
int rbss;

void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  rbss = 7;
  rfp = 1024;
  
  int ret = yyparse();

  node_t* t = (node_t*)arvore;
  //print_program(t->code);

  cfg_t* cfg = generate_cfg(t->code);
  //print_cfg(cfg);
  print_cfg_dot(cfg);

  destroy_cfg(cfg);
  destroy_stack(table_stack);
  destroy_iloc_program(t->code);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  //return ret;

  return 0; // Na ausencia de qualquer erro, o programa deve retornar o valor zero.
}