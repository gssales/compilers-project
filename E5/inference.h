#ifndef _INFERENCE_H_
#define _INFERENCE_H_

#include <stdio.h>
#include "arvore.h"
#include "tabela.h"

void erro_inferencia(int erro, node_t* exprA, node_t* exprB);
int infere_tipo(node_t* exprA, node_t* exprB);
void infere_tipo_atrib(int type, node_t* expr);
void infere_tipo_inicializacao(node_t* lista_local_var, int type);
int get_lineno(node_t* expr);

#endif //_INFERENCE_H_
