#ifndef _INFERENCE_H_
#define _INFERENCE_H_

#include <stdio.h>
#include "arvore.h"
#include "tabela.h"

void inference_error(int erro, node_t* exprA, node_t* exprB);
symbol_type_t infer_type(node_t* exprA, node_t* exprB);
void infer_type_attribution(int type, node_t* expr);
void infer_type_initialization(node_t* lista_local_var, int type);
int get_lineno(node_t* expr);

#endif //_INFERENCE_H_
