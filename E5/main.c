/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>

#include "tabela.h"
#include "iloc.h"
#include "parser.tab.h"
extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void* table_stack;
void* strstack;

void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  /*
  iloc_program_t* program = create_iloc_program();

  iloc_code_t* code_nop = create_iloc_code(NOP);
  int l0 = new_label();
  int l1 = new_label();
  code_nop->label = l1;
  push_iloc_code(program, code_nop);

  int r0 = new_reg();
  int r1 = new_reg();
  int r2 = new_reg();
  int r3 = new_reg();
  iloc_code_t* code_add = create_iloc_code3op(ADD, TEMPORARY, r1, TEMPORARY, r2, TEMPORARY, r3);
  push_iloc_code(program, code_add);

  iloc_code_t* code_store_ai = create_iloc_code3op(STORE_AI, IMMEDIATE, 12312, TEMPORARY, 3262, LABEL, 134135);
  push_iloc_code(program, code_store_ai);

  iloc_code_t* code_load = create_iloc_code2op(LOAD, IMMEDIATE, 2, TEMPORARY, 3262);
  push_iloc_code(program, code_load);

  iloc_code_t* code_jump = create_iloc_code1op(JUMP, LABEL, 2);
  push_iloc_code(program, code_jump);

  iloc_code_t* code_cbr = create_iloc_code3op(CBR, IMMEDIATE, 12312, TEMPORARY, 3262, LABEL, 134135);
  push_iloc_code(program, code_cbr);

  iloc_code_t* code_lt = create_iloc_code3op(CMP_LT, IMMEDIATE, 12312, TEMPORARY, 3262, LABEL, 134135);
  push_iloc_code(program, code_lt);

  print_program(program);

  destroy_iloc_program(program);*/
  
  int ret = yyparse();
  exporta(arvore);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  //return ret;
  return 0; // Na ausencia de qualquer erro, o programa deve retornar o valor zero.
}