#ifndef _ASM_H_
#define _ASM_H_

#include "tabela.h"
#include "iloc.h"

void map_asm_op(iloc_code_t* code);
char* map_arg_type_asm(arg_type_t type, int reg);
void generateAsm(table_t* data, iloc_program_t* program);

typedef enum {
	EBX, ECX, EDX, ESI, EDI, 
	R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D
} asm_reg_t; // 13 available regs

asm_reg_t asm_reg(int iloc_reg);

#endif //_ASM_H_