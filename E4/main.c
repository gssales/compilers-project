/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>

#include "tabela.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void* pilha_tabelas = NULL;
void* pilha_str = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  
  int ret = yyparse();
  //exporta(arvore);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  //return ret;
  return 0; // Na ausencia de qualquer erro, o programa deve retornar o valor zero.
}