#include <stdlib.h>
#include <stdio.h>
#include "asm.h"



const char* map_asm_op(iloc_op_t op) {
    switch(op) {
        /* INSTRUCOES ILOC POSSIVEIS (IMPLEMENTADAS NO PARSER):
            NOP, I2I, JUMP, JUMP_I, ADD, SUB, MULT, DIV, AND, OR, ADD_I, LOAD_I, STORE_AI, LOAD_AI, 
            CBR, CMP_EQ, CMP_NE, CMP_LT, CMP_GT, CMP_LE, CMP_GE
        */
        case NOP:
        case I2I:

        case JUMP:
        case JUMP_I:

        case ADD:
        case SUB:
        case MULT:
        case DIV:
        case AND:
        case OR:
        
        case ADD_I:

        case LOAD_I:
        case STORE_AI:
        case LOAD_AI:

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