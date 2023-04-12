/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>

#include "tabela.h"
#include "iloc.h"
#include "arvore.h"
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
  // print_program(t->code);

  stack_t* s = (stack_t*)table_stack;
  // print_table(get_table(s, 0));

  generateAsm(get_table(s, 0), t->code);

  //exporta(arvore);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  //return ret;

  return 0; // Na ausencia de qualquer erro, o programa deve retornar o valor zero.
}