#include "parser.tab.h"

char* tk_type_to_string(int tk_type) {
  switch(tk_type) {
    case 0:                 return "CHAR_ESPECIAL   "; break;
    case TK_PR_INT:         return "TK_PR_INT       "; break;
    case TK_PR_FLOAT:       return "TK_PR_FLOAT     "; break;
    case TK_PR_BOOL:        return "TK_PR_BOOL      "; break;
    case TK_PR_CHAR:        return "TK_PR_CHAR      "; break;
    case TK_PR_IF:          return "TK_PR_IF        "; break;
    case TK_PR_THEN:        return "TK_PR_THEN      "; break;
    case TK_PR_ELSE:        return "TK_PR_ELSE      "; break;
    case TK_PR_WHILE:       return "TK_PR_WHILE     "; break;
    case TK_PR_INPUT:       return "TK_PR_INPUT     "; break;
    case TK_PR_OUTPUT:      return "TK_PR_OUTPUT    "; break;
    case TK_PR_RETURN:      return "TK_PR_RETURN    "; break;
    case TK_PR_FOR:         return "TK_PR_FOR       "; break;
    case TK_OC_LE:          return "TK_OC_LE        "; break;
    case TK_OC_GE:          return "TK_OC_GE        "; break;
    case TK_OC_EQ:          return "TK_OC_EQ        "; break;
    case TK_OC_NE:          return "TK_OC_NE        "; break;
    case TK_OC_AND:         return "TK_OC_AND       "; break;
    case TK_OC_OR:          return "TK_OC_OR        "; break;
    case TK_LIT_INT:        return "TK_LIT_INT      "; break;
    case TK_LIT_FLOAT:      return "TK_LIT_FLOAT    "; break;
    case TK_LIT_FALSE:      return "TK_LIT_FALSE    "; break;
    case TK_LIT_TRUE:       return "TK_LIT_TRUE     "; break;
    case TK_LIT_CHAR:       return "TK_LIT_CHAR     "; break;
    case TK_IDENTIFICADOR:  return "TK_IDENTIFICADOR"; break;
    case TK_ERRO:           return "TK_ERRO         "; break;
  }
}

lexvalue_t* create_lexvalue(int lineno, int token, char* lexema) {
  lexvalue_t* new_lexvalue = malloc(sizeof(lexvalue_t));

  if (new_lexvalue != NULL) {
    new_lexvalue->line_number = lineno;
    new_lexvalue->tk_type = token;

    new_lexvalue->lexema = malloc((strlen(lexema) + 1) * sizeof(char));
    if (new_lexvalue->lexema != NULL)
      strcpy(new_lexvalue->lexema, lexema);

    switch (token) {
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
      default:
        new_lexvalue->tk_value.s = malloc((strlen(lexema)+1) * sizeof(char));
        strcpy(new_lexvalue->tk_value.s, lexema);
    }
  }

  return new_lexvalue;
}

lexvalue_t* copy_lexvalue(lexvalue_t* lexvalue) {
  return create_lexvalue(lexvalue->line_number, lexvalue->tk_type, lexvalue->lexema);
}

void destroy_lexvalue(lexvalue_t* lexvalue) {
  if (lexvalue != NULL) {
    free(lexvalue->lexema);
    lexvalue->lexema = NULL;
    switch(lexvalue->tk_type) {
      case TK_LIT_INT:
      case TK_LIT_FLOAT:
      case TK_LIT_CHAR:
      case TK_LIT_TRUE:
      case TK_LIT_FALSE:
        break;
      default:
        free(lexvalue->tk_value.s);
        lexvalue->tk_value.s = NULL;
    }
    free(lexvalue);
  }
}

void print_lexvalue(lexvalue_t* lexvalue) {
  if (lexvalue) {
    printf("l%d: lexema: %s\t%s\ttk_value: ", lexvalue->line_number, lexvalue->lexema, tk_type_to_string(lexvalue->tk_type));
    print_tk_value(lexvalue);
    printf("\n");
    fflush(stdout);
  }
}

void print_tk_value(lexvalue_t* lexvalue) {
  if (lexvalue != NULL) {
    switch (lexvalue->tk_type) {
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
      default:
        printf("%s", lexvalue->tk_value.s);
    }
  }
}
