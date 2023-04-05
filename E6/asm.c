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
        //addl	$500, %eax // imediato
        case ADD:
        case ADD_I:

        // subl	-4(%rbp), %eax // deslocamento
        // subl	$500, %eax // imediato
        case SUB:
        
        case MULT:
        case DIV:
        case AND:
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