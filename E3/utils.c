#include "parser.tab.h"

valor_lexico* create_lexvalue(int lineno, int token, char* lexema) {
  valor_lexico* new_lexvalue;
  new_lexvalue->line_number = lineno;
  new_lexvalue->tk_type = token;
  switch (token) {
    //case 0: // CHAR ESPECIAL
    //  new_lexvalue->tk_value.s = lexema;
    case TK_IDENTIFICADOR:
      new_lexvalue->tk_value.s = lexema;
    case TK_LIT_INT:
      new_lexvalue->tk_value.i = atoi(lexema);
    case TK_LIT_FLOAT:
      new_lexvalue->tk_value.f = atof(lexema);
    case TK_LIT_CHAR:
      new_lexvalue->tk_value.c = lexema[0];
    case TK_LIT_TRUE:
      new_lexvalue->tk_value.i = 1;
    case TK_LIT_FALSE:
      new_lexvalue->tk_value.i = 0;
    //case TK_PR_INT:
      //new_lexvalue->tk_value.s = lexema;
  }
  print_lexvalue(new_lexvalue);
}

char* tk_type_to_string(int tk_type) {
  switch(tk_type) {
    case TK_PR_INT:
      return "TK_PR_INT";
      break;
    case TK_PR_FLOAT:
      return "TK_PR_FLOAT";
      break;
    case TK_PR_BOOL:
      return "TK_PR_BOOL";
      break;
    case TK_PR_CHAR:
      return "TK_PR_CHAR";
      break;
    case TK_PR_IF:
      return "TK_PR_IF";
      break;
    case TK_PR_THEN:
      return "TK_PR_THEN";
      break;
    case TK_PR_ELSE:
      return "TK_PR_ELSE";
      break;
    case TK_PR_WHILE:
      return "TK_PR_WHILE";
      break;
    case TK_PR_INPUT:
      return "TK_PR_INPUT";
      break;
    case TK_PR_OUTPUT:
      return "TK_PR_OUTPUT";
      break;
    case TK_PR_RETURN:
      return "TK_PR_RETURN";
      break;
    case TK_PR_FOR:
      return "TK_PR_FOR";
      break;
    case TK_OC_LE:
      return "TK_OC_LE";
      break;
    case TK_OC_GE:
      return "TK_OC_GE";
      break;
    case TK_OC_EQ:
      return "TK_OC_EQ";
      break;
    case TK_OC_NE:
      return "TK_OC_NE";
      break;
    case TK_OC_AND:
      return "TK_OC_AND";
      break;
    case TK_OC_OR:
      return "TK_OC_OR";
      break;
    case TK_LIT_INT:
      return "TK_LIT_INT";
      break;
    case TK_LIT_FLOAT:
      return "TK_LIT_FLOAT";
      break;
    case TK_LIT_FALSE:
      return "TK_LIT_FALSE";
      break;
    case TK_LIT_TRUE:
      return "TK_LIT_TRUE";
      break;
    case TK_LIT_CHAR:
      return "TK_LIT_CHAR";
      break;
    case TK_IDENTIFICADOR:
      return "TK_IDENTIFICADOR";
      break;
    case TK_ERRO:
      return "TK_ERRO";
      break;
  }
}

void print_lexvalue(valor_lexico* lexvalue) {
  printf("l%d: %s\n", lexvalue->line_number, tk_type_to_string(lexvalue->tk_type));

  // prints do valor dependendo do tipo
}