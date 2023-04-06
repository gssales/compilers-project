#ifndef _ASM_H_
#define _ASM_H_

#include "tabela.h"
#include "iloc.h"

const char* map_asm_op(iloc_code_t* code);
const char* map_asm_format(iloc_op_t op);
char* map_arg_type_asm(arg_type_t type, int reg);
void generateAsm(table_t* data, iloc_program_t* program);

asm_reg_t asm_reg(int iloc_reg);

typedef enum {
	EBX, ECX, EDX, ESI, EDI, 
	R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D
} asm_reg_t; // 13 available regs

typedef enum {
  ONE,        /* l0 */
  ONE_TO_ONE_INST, /* c0, c1 */
  ONE_TO_DISP_INST, /* c0, -d(c1) */
  DISP_TO_ONE_INST /* -d(c0), c1 */
} asm_code_format_t;

#endif //_ASM_H_