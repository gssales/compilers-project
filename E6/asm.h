#ifndef _ASM_H_
#define _ASM_H_

#include "tabela.h"

const char* map_asm_op(iloc_op_t op);
const char* map_asm_format(iloc_op_t op);
char* map_arg_type_asm(arg_type_t type, int reg);
void generateAsm(table_t* data, iloc_program_t* code);



#endif //_ASM_H_