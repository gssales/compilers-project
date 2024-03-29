#include "parser.tab.h"

char* tk_type_to_string(int tk_type) {
  switch(tk_type) {
    case 0:                 return "CHAR_ESPECIAL   ";     break;
    case TK_PR_INT:         return "TK_PR_INT       ";         break;
    case TK_PR_FLOAT:       return "TK_PR_FLOAT     ";       break;
    case TK_PR_BOOL:        return "TK_PR_BOOL      ";        break;
    case TK_PR_CHAR:        return "TK_PR_CHAR      ";        break;
    case TK_PR_IF:          return "TK_PR_IF        ";          break;
    case TK_PR_THEN:        return "TK_PR_THEN      ";        break;
    case TK_PR_ELSE:        return "TK_PR_ELSE      ";        break;
    case TK_PR_WHILE:       return "TK_PR_WHILE     ";       break;
    case TK_PR_INPUT:       return "TK_PR_INPUT     ";       break;
    case TK_PR_OUTPUT:      return "TK_PR_OUTPUT    ";      break;
    case TK_PR_RETURN:      return "TK_PR_RETURN    ";      break;
    case TK_PR_FOR:         return "TK_PR_FOR       ";         break;
    case TK_OC_LE:          return "TK_OC_LE        ";          break;
    case TK_OC_GE:          return "TK_OC_GE        ";          break;
    case TK_OC_EQ:          return "TK_OC_EQ        ";          break;
    case TK_OC_NE:          return "TK_OC_NE        ";          break;
    case TK_OC_AND:         return "TK_OC_AND       ";         break;
    case TK_OC_OR:          return "TK_OC_OR        ";          break;
    case TK_LIT_INT:        return "TK_LIT_INT      ";        break;
    case TK_LIT_FLOAT:      return "TK_LIT_FLOAT    ";      break;
    case TK_LIT_FALSE:      return "TK_LIT_FALSE    ";      break;
    case TK_LIT_TRUE:       return "TK_LIT_TRUE     ";       break;
    case TK_LIT_CHAR:       return "TK_LIT_CHAR     ";       break;
    case TK_IDENTIFICADOR:  return "TK_IDENTIFICADOR";  break;
    case TK_ERRO:           return "TK_ERRO         ";           break;
  }
}

valor_lexico* create_lexvalue(int lineno, int token, char* lexema) {
  valor_lexico* new_lexvalue;
  new_lexvalue = malloc(sizeof(valor_lexico));
  new_lexvalue->line_number = lineno;
  new_lexvalue->tk_type = token;
  new_lexvalue->str = malloc((strlen(lexema) + 1) * sizeof(char));
  if (new_lexvalue->str != NULL)
    strcpy(new_lexvalue->str, lexema);
  switch (token) {
    case 0: // chars especiais
    case TK_OC_LE:
    case TK_OC_GE:
    case TK_OC_NE:
    case TK_OC_EQ:
    case TK_OC_AND:
    case TK_OC_OR:
    case TK_IDENTIFICADOR:
    case TK_PR_INT:
    case TK_PR_FLOAT:
    case TK_PR_BOOL:
    case TK_PR_CHAR:
    case TK_PR_IF:
    case TK_PR_THEN:
    case TK_PR_ELSE:
    case TK_PR_WHILE:
    case TK_PR_INPUT:
    case TK_PR_OUTPUT:
    case TK_PR_RETURN:
    case TK_PR_FOR:
      new_lexvalue->tk_value.s = malloc((strlen(lexema)+1) * sizeof(char));
      strcpy(new_lexvalue->tk_value.s, lexema);
      break;
    case TK_LIT_INT:
      new_lexvalue->tk_value.i = atoi(lexema);
      break;
    case TK_LIT_FLOAT:
      new_lexvalue->tk_value.f = atof(lexema);
      break;
    case TK_LIT_CHAR:
      new_lexvalue->tk_value.c = lexema[1];
      break;
    case TK_LIT_TRUE:
      new_lexvalue->tk_value.i = 1;
      break;
    case TK_LIT_FALSE:
      new_lexvalue->tk_value.i = 0;
      break;
  }
  return new_lexvalue;
}

valor_lexico* copy_lexvalue(valor_lexico* value) {
  valor_lexico *lex = malloc(sizeof(valor_lexico));
  lex->line_number = value->line_number;
  lex->tk_type = value->tk_type;
  lex->str = malloc((strlen(value->str) + 1) * sizeof(char));
  if (lex->str != NULL)
    strcpy(lex->str, value->str);

  switch (lex->tk_type) {
  case TK_LIT_INT:
  case TK_LIT_FLOAT:
  case TK_LIT_CHAR:
  case TK_LIT_TRUE:
  case TK_LIT_FALSE:
    lex->tk_value = value->tk_value;
    break;
  default:
    lex->tk_value.s = malloc((strlen(value->tk_value.s)+1) * sizeof(char));
    strcpy(lex->tk_value.s, value->tk_value.s);
  }

  return lex;
}

void destroy_lexvalue(valor_lexico* valor_lexico) {
  //print_lexvalue(valor_lexico);
  if (valor_lexico != NULL) {
    free(valor_lexico->str);
    switch(valor_lexico->tk_type) {
      case TK_LIT_INT:
      case TK_LIT_FLOAT:
      case TK_LIT_CHAR:
      case TK_LIT_TRUE:
      case TK_LIT_FALSE:
        break;
      default:
        //printf("%s after\n", valor_lexico->tk_value.s);
        free(valor_lexico->tk_value.s);
    }
    free(valor_lexico);
  }
}

void print_lexvalue(valor_lexico* lexvalue) {
  //printf("l%d: %s ", lexvalue->line_number, tk_type_to_string(lexvalue->tk_type));
  printf("%s\ttk_value: ", tk_type_to_string(lexvalue->tk_type));
  print_tk_value(lexvalue);
  printf("\n");
  fflush(stdout);
}

void print_tk_value(valor_lexico* lexvalue) {
  switch (lexvalue->tk_type) {
    case 0: // chars especiais
    case TK_OC_LE:
    case TK_OC_GE:
    case TK_OC_NE:
    case TK_OC_EQ:
    case TK_OC_AND:
    case TK_OC_OR:
    case TK_IDENTIFICADOR:
    case TK_PR_INT:
    case TK_PR_FLOAT:
    case TK_PR_BOOL:
    case TK_PR_CHAR:
    case TK_PR_IF:
    case TK_PR_THEN:
    case TK_PR_ELSE:
    case TK_PR_WHILE:
    case TK_PR_INPUT:
    case TK_PR_OUTPUT:
    case TK_PR_RETURN:
    case TK_PR_FOR:
      printf("%s", lexvalue->tk_value.s);
      break;
    case TK_LIT_TRUE:
    case TK_LIT_FALSE:
    case TK_LIT_INT:
      printf("%d", lexvalue->tk_value.i);
      break;
    case TK_LIT_FLOAT:
      printf("%f", lexvalue->tk_value.f);
      break;
    case TK_LIT_CHAR:
      printf("%c", lexvalue->tk_value.c);
      break;
  }
}
