#include "inference.h"


void erro_inferencia(int erro, node_t* exprA, node_t* exprB) {
  switch (erro) {
    case ERR_CHAR_TO_INT: //2.4
      printf("Linha %d - ERR_CHAR_TO_INT: Tentativa de coerção da expressão '%s' do tipo char para o tipo int\n", get_lineno(exprB), "CHAR");
      break;
    case ERR_CHAR_TO_FLOAT: //2.4
      printf("Linha %d - ERR_CHAR_TO_FLOAT: Tentativa de coerção da expressão '%s' do tipo char para o tipo float\n", get_lineno(exprB), "CHAR");
      break;
    case ERR_CHAR_TO_BOOL: //2.4
      printf("Linha %d - ERR_CHAR_TO_BOOL: Tentativa de coerção da expressão '%s' do tipo char para o tipo bool\n", get_lineno(exprB), "CHAR");
      break;
    case ERR_CHAR_VECTOR: //2.4
      printf("Linha %d - ERR_CHAR_VECTOR: Arranjo '%s' declarado com o tipo char\n", get_lineno(exprB), "CHAR");
      break;
    case ERR_X_TO_CHAR: //2.4
      printf("Linha %d - ERR_X_TO_CHAR: Tentativa de coerção da variável '%s' do tipo int/float/bool para o tipo char\n", get_lineno(exprB), "CHAR");
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

int _infere_tipo(int tipo1, int tipo2, node_t* exprA, node_t* exprB) {
  int tipo_inferido = TYPE_UNDEFINED;
  switch (tipo1) {
  case TYPE_FLOAT:
    switch (tipo2) {
    case TYPE_INT:
    case TYPE_BOOL:
      tipo_inferido = TYPE_FLOAT;
      break;
    case TYPE_CHAR: 
      erro_inferencia(ERR_CHAR_TO_FLOAT, exprA, exprB);
    }
    break;
  case TYPE_INT:
    switch (tipo2) {
    case TYPE_FLOAT:
      tipo_inferido = TYPE_FLOAT;
      break;
    case TYPE_BOOL:
      tipo_inferido = TYPE_INT;
      break;
    case TYPE_CHAR: 
      erro_inferencia(ERR_CHAR_TO_INT, exprA, exprB);
    }
    break;
  case TYPE_BOOL:
    switch (tipo2) {
    case TYPE_FLOAT:
      tipo_inferido = TYPE_FLOAT;
      break;
    case TYPE_INT:
      tipo_inferido = TYPE_INT;
      break;
    case TYPE_CHAR: 
      erro_inferencia(ERR_CHAR_TO_BOOL, exprA, exprB);
    }
    break;
  case TYPE_CHAR:
    switch (tipo2) {
    case TYPE_FLOAT:
      erro_inferencia(ERR_CHAR_TO_FLOAT, exprA, exprB);
      break;
    case TYPE_INT:
      erro_inferencia(ERR_CHAR_TO_INT, exprA, exprB);
      break;
    case TYPE_BOOL: 
      erro_inferencia(ERR_CHAR_TO_BOOL, exprA, exprB);
    }
    break;    
  }
  return tipo_inferido;
}

int infere_tipo(node_t* exprA, node_t* exprB) {
  if (exprA->type == TYPE_UNDEFINED || exprB->type == TYPE_UNDEFINED) {
    printf("ERRO expressão não tem tipo");
    exit(1);
  }

  if (exprA->type == exprB->type)
    return exprA->type;

  return _infere_tipo(exprA->type, exprB->type, exprA, exprB);
}

void infere_tipo_atrib(int type, node_t* expr) {
  if (type == TYPE_CHAR) {
    if (expr->type != TYPE_CHAR) {
      erro_inferencia(ERR_X_TO_CHAR, NULL, expr);
    }
  } else {
    _infere_tipo(type, expr->type, NULL, expr);
  }
}

void infere_tipo_inicializacao(node_t* lista_local_var, int type) {
  if (lista_local_var != NULL && lista_local_var->flag == COMANDO) {
    for (int i = 0; i < lista_local_var->count_children; i++) {
      node_t* child = lista_local_var->children[i];

      if (child->flag == VAR_INICIALIZACAO) {
        child->type = type;
        lista_local_var->type = type;
      }

      if (child->flag == LIT_INICIALIZACAO)
        if (type == TYPE_CHAR) {
          if (child->type != TYPE_CHAR) {
            erro_inferencia(ERR_X_TO_CHAR, NULL, lista_local_var);
          }
        } else {
          _infere_tipo(type, child->type, NULL, lista_local_var);
        }

      if (child->flag == COMANDO)
        infere_tipo_inicializacao(child, type);
    }
  }
}
