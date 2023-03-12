#include "inference.h"


void inference_error(int erro, node_t* exprA, node_t* exprB) {
  switch (erro) {
    case ERR_CHAR_TO_INT: //2.4
      printf("Linha %d - ERR_CHAR_TO_INT: Tentativa de coerção da expressão do tipo char para o tipo int\n", get_lineno(exprB));
      break;
    case ERR_CHAR_TO_FLOAT: //2.4
      printf("Linha %d - ERR_CHAR_TO_FLOAT: Tentativa de coerção da expressão do tipo char para o tipo float\n", get_lineno(exprB));
      break;
    case ERR_CHAR_TO_BOOL: //2.4
      printf("Linha %d - ERR_CHAR_TO_BOOL: Tentativa de coerção da expressão do tipo char para o tipo bool\n", get_lineno(exprB));
      break;
    case ERR_X_TO_CHAR: //2.4
      printf("Linha %d - ERR_X_TO_CHAR: Tentativa de coerção da variável do tipo int/float/bool para o tipo char\n", get_lineno(exprB));
      break;
  }
  exit(erro);
}

int get_lineno(node_t* expr) {
  int line_number = -1;
  if (expr != NULL) {
    if (expr->value != NULL)
      line_number = expr->value->line_number;
    else
      for (int i = 0; i < expr->count_children; i++) {
        line_number = get_lineno(expr->children[i]);
        if (line_number > 0) break;
      }
  }
  return line_number;
}

symbol_type_t _infer_type(int type1, int type2, node_t* exprA, node_t* exprB) {
  symbol_type_t infered_type = TYPE_UNDEFINED;
  switch (type1) {
  case TYPE_FLOAT:
    switch (type2) {
    case TYPE_INT:
    case TYPE_BOOL:
      infered_type = TYPE_FLOAT;
      break;
    case TYPE_CHAR: 
      inference_error(ERR_CHAR_TO_FLOAT, exprA, exprB);
    }
    break;
  case TYPE_INT:
    switch (type2) {
    case TYPE_FLOAT:
      infered_type = TYPE_FLOAT;
      break;
    case TYPE_BOOL:
      infered_type = TYPE_INT;
      break;
    case TYPE_CHAR: 
      inference_error(ERR_CHAR_TO_INT, exprA, exprB);
    }
    break;
  case TYPE_BOOL:
    switch (type2) {
    case TYPE_FLOAT:
      infered_type = TYPE_FLOAT;
      break;
    case TYPE_INT:
      infered_type = TYPE_INT;
      break;
    case TYPE_CHAR: 
      inference_error(ERR_CHAR_TO_BOOL, exprA, exprB);
    }
    break;
  case TYPE_CHAR:
    switch (type2) {
    case TYPE_FLOAT:
      inference_error(ERR_CHAR_TO_FLOAT, exprA, exprB);
      break;
    case TYPE_INT:
      inference_error(ERR_CHAR_TO_INT, exprA, exprB);
      break;
    case TYPE_BOOL: 
      inference_error(ERR_CHAR_TO_BOOL, exprA, exprB);
    }
    break;    
  }
  return infered_type;
}

symbol_type_t infer_type(node_t* exprA, node_t* exprB) {
  if (exprA->sym_type == TYPE_UNDEFINED || exprB->sym_type == TYPE_UNDEFINED) {
    printf("ERRO expressão não tem tipo");
    exit(1);
  }

  if (exprA->sym_type == exprB->sym_type)
    return exprA->sym_type;

  return _infer_type(exprA->sym_type, exprB->sym_type, exprA, exprB);
}

void infer_type_attribution(int type, node_t* expr) {
  if (type == TYPE_CHAR) {
    if (expr->sym_type != TYPE_CHAR) {
      inference_error(ERR_X_TO_CHAR, NULL, expr);
    }
  } else {
    _infer_type(type, expr->sym_type, NULL, expr);
  }
}

void infer_type_initialization(node_t* lista_local_var, int type) {
  if (lista_local_var != NULL && lista_local_var->ast_type == AST_COMMAND) {
    for (int i = 0; i < lista_local_var->count_children; i++) {
      node_t* child = lista_local_var->children[i];

      if (child->ast_type == AST_VAR_INITIALIZED) {
        child->sym_type = type;
        lista_local_var->sym_type = type;
      }

      if (child->ast_type == AST_LIT_DECLARED)
        if (type == TYPE_CHAR) {
          if (child->sym_type != TYPE_CHAR) {
            inference_error(ERR_X_TO_CHAR, NULL, lista_local_var);
          }
        } else {
          _infer_type(type, child->sym_type, NULL, lista_local_var);
        }

      if (child->ast_type == AST_COMMAND)
        infer_type_initialization(child, type);
    }
  }
}
