#include <stdlib.h>
#include <stdio.h>
#include "asm.h"

// REFERENCIA BOA:
// https://web.stanford.edu/class/cs107/guide/x86-64.html

const char* map_asm_op(iloc_op_t op) {
    switch(op) {
        /* INSTRUCOES ILOC POSSIVEIS (IMPLEMENTADAS NO PARSER):
            NOP, I2I, JUMP, JUMP_I, ADD, SUB, MULT, DIV, AND, OR, ADD_I, LOAD_I, STORE_AI, LOAD_AI, 
            CBR, CMP_EQ, CMP_NE, CMP_LT, CMP_GT, CMP_LE, CMP_GE
        */
       
        case NOP:
        case I2I:

        // jmp   *%eax
        case JUMP:

        // jmp	.L3
        case JUMP_I:

        //addl	%edx, %eax
        //addl	$500, %eax
        case ADD:
        case ADD_I:

        // subl	-4(%rbp), %eax
        // subl	$500, %eax
        case SUB:
        
        // imull	-4(%rbp), %eax
        // imull	$500, %eax, %eax
        case MULT:

        // https://stackoverflow.com/questions/17170388/trying-to-understand-the-assembly-instruction-cltd-on-x86
        // https://stackoverflow.com/questions/60010410/understanding-the-x86-idivl-instruction
        // 1. load pra eax
        // 2. cltd (extende edx para edx:eax)
        // 3. idivl ____ (divide edx:eax pelo operando) (resultado: quociente @eax, resto @edx)
        //movl	-8(%rbp), %eax
	    //cltd
	    //idivl	-4(%rbp)
        case DIV:

        // ignorar .s de teste (usa comparacao e jumps), tentar instr. 'and':
        // and src, dst       # dst &= src
        case AND:

        // ignorar .s de teste (usa comparacao e jumps), tentar instr. 'or':
        // or src, dst        # dst |= src
        case OR:

        //movl	$100, -12(%rbp)
        case LOAD_I:

        //movl	-4(%rbp), %eax
        case LOAD_AI:

        //movl	%eax, -12(%rbp)
        case STORE_AI:

        case CBR:

        case CMP_EQ:
        case CMP_NE:
        case CMP_LT:
        case CMP_GT:
        case CMP_LE:
        case CMP_GE:
    }
}

const char* map_asm_format(iloc_op_t op);
char* map_arg_type_asm(arg_type_t type, int reg);
void generateAsm(table_t* data, iloc_program_t* code);