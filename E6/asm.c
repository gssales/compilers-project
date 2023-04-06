#include <stdlib.h>
#include <stdio.h>
#include "asm.h"

// REFERENCIA BOA:
// https://web.stanford.edu/class/cs107/guide/x86-64.html

const char* map_asm_op(iloc_code_t* code) {
    switch(code->op) {
        /* INSTRUCOES ILOC POSSIVEIS (IMPLEMENTADAS NO PARSER):
            NOP, I2I, JUMP, JUMP_I, ADD, SUB, MULT, DIV, AND, OR, ADD_I, LOAD_I, STORE_AI, LOAD_AI, 
            CBR, CMP_EQ, CMP_NE, CMP_LT, CMP_GT, CMP_LE, CMP_GE
        */

        // nop
        case NOP:

        // (nao tenho certeza):
        // mov src, dst              # general form of instruction dst = src
        // mov %eax, %ebx
        case I2I:

        // jmp   *%eax
        // jmp	.L3
        case JUMP:
        case JUMP_I:
        	printf("\tjmp\t%s\n", map_arg_type_asm(code->arg_types[0], code->args[0]));
        	break;

        //addl	%edx, %eax
        //addl	$500, %eax
        case ADD:
        case ADD_I:
        	printf("\tmovl\t%s, %s\n", map_arg_type_asm(code->arg_types[0], code->args[0]), map_arg_type_asm(code->arg_types[2], code->args[2]));
        	printf("\taddl\t%s, %s\n", map_arg_type_asm(code->arg_types[1], code->args[1]), map_arg_type_asm(code->arg_types[2], code->args[2]));
        	break;

        // subl	-4(%rbp), %eax
        // subl	$500, %eax
        case SUB:
        	printf("\tmovl\t%s, %s\n", map_arg_type_asm(code->arg_types[0], code->args[0]), map_arg_type_asm(code->arg_types[2], code->args[2]));
        	printf("\tsubl\t%s, %s\n", map_arg_type_asm(code->arg_types[1], code->args[1]), map_arg_type_asm(code->arg_types[2], code->args[2]));
        	break;
        
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
        // HACK PRA TRANSFORMAR AND DE 2 REG EM 3 DO ILOC (r1,r2 -> r3): MOVL EXTRA PRA POR RESULTADO EM R3
        // and "r1", "r2"   # resultado fica em "r2"
        // movl "r2", "r3"
        case AND:

        // ignorar .s de teste (usa comparacao e jumps), tentar instr. 'or':
        // or src, dst        # dst |= src
        // HACK PRA TRANSFORMAR OR DE 2 REG EM 3 DO ILOC (r1,r2 -> r3): MOVL EXTRA PRA POR RESULTADO EM R3
        // or "r1", "r2"   # resultado fica em "r2"
        // movl "r2", "r3"
        case OR:

        //movl	$100, -12(%rbp)
        case LOAD_I:

        //movl	-4(%rbp), %eax
        case LOAD_AI:

        //movl	%eax, -12(%rbp)
        case STORE_AI:

        // movl "r1", %eax  # carrega r1 p/ eax
        // cmpl	"r2", %eax  # compara r2 com r1(eax) + set flags de condicao
        case CMP_EQ:
            // sete %al
        case CMP_NE:
            // setne %al
        case CMP_LT:
            // setl %al
        case CMP_GT:
            // setg %al
        case CMP_LE:
            // setle %al
        case CMP_GE:
            // setge %al
        // movzbl %al, "r3"     # resultado em "r3"

        // ILOC: cbr "r1" -> l1, l2
        // cmpl	$0, "r1"    
	    // je   "l2"        # se == 0, jump pra lf
        // jmp  "l1"        # se != 0, incondicional pra lt
        case CBR:

    }
}

char* map_arg_type_asm(arg_type_t type, int reg) {
	const char* base_regs[] = { "rpc", "%%rbp", "%%rsp", "%%rip" };
	const char* regs[] = { "%%ebx", "%%ecx", "%%edx", "%%esi", "%%edi",
			"%%r8d", "%%r9d", "%%r10d", "%%r11d", "%%r12d", "%%r13d", "%%r14d", "%%r15d" };
	if (reg >= 0) {
		if (type == TEMPORARY)
			return regs[asm_reg(reg)];
		else {
			char* c = malloc(10 * sizeof(char));
			if (type == LABEL)
				snprintf(c, 10, ".L%d", reg);
			else
				snprintf(c, 10, "$%d", reg);
			return c;
		}
	}
	if (type == TEMPORARY && reg >= -4) {
		return regs[reg +4];
	}
	return "";
}

int used_regs[13];
asm_reg_t asm_reg(int iloc_reg) {
	static int count_reg = 0;
	for (int i = 0; i < 13; i++)
		if (used_regs[i] == iloc_reg)
			return i;
	count_reg = (count_reg + 1) % 13;
	used_regs[count_reg] = iloc_reg;
	return count_reg;
}

void generateAsm(table_t* data, iloc_program_t* program) {
	if (data != NULL && program != NULL) {

		// generate data section
		printf("\t.bss\n");
		for (int i = 0; i < data->count_symbols; i++) {
			symbol_t* s = data->list[i]->symbol;
			if (s->sym_nature == SYM_VARIAVEL) {
				printf("\t.globl\t%s\n", s->value->tk_value.s);
				printf("\t.align %d\n", s->sizeB);
				printf("\t.type\t%s, @object\n", s->value->tk_value.s);
				printf("\t.size\t%s, %d\n", s->value->tk_value.s, s->sizeB);
				printf("%s:\n", s->value->tk_value.s);
				printf("\t.zero\t%d\n", s->sizeB);
			}
		}

		char* asm_label = NULL;

		// generate code section
		printf("\t.text\n");
		iloc_code_t* code = program->head;
		while (code != NULL) {
			iloc_code_t* next = code->next;

			if (code->asm_label != NULL) {
				asm_label = code->asm_label;
				printf("\t.globl\t%s\n", asm_label);
				printf("\t.type\t%s, @function\n", asm_label);
				printf("%s:\n", asm_label);
				printf("\tpushq\t%%rbp\n");
			}
			else if (code->is_retval) {
				printf("\tmovl\t$%d, %%eax\n", 100);
			} 
			else if (code->is_ret) {
				printf("\tpopq\t%%rbp\n");
				printf("\tret\n");
			} 
			else {
				// map code
			}

			if (code->is_end_function) {
				printf("\t.size\t%s, .-%s\n", asm_label, asm_label);
				asm_label = NULL;
			}

			code = next;
		}
	}

}