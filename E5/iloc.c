#include <stdlib.h>
#include <stdio.h>
#include "iloc.h"

const char* map_iloc_op(iloc_op_t op) {
  const char* iloc_op_consts[] = {
    "nop", "halt", "add", "addI", "sub", "subI", "rsubI", "mult", "multI", "div", "divI",
    "rdivI", "lshift", "lshiftI", "rshift", "rshiftI", "and", "andI", "or", "orI",
    "xor", "xorI", "load", "loadI", "loadAI", "loadA0", "cload", "cloadAI",
    "cloadA0", "store", "storeAI", "storeAO", "cstore", "cstoreAI", "cstoreAO",
    "i2i", "c2c", "c2i", "i2c", "jumpI", "jump", "cbr", "cmp_LT", "cmp_LE",
    "cmp_EQ", "cmp_NE", "cmp_GT", "cmp_GE"
  };
  return iloc_op_consts[op];
}

const char* map_iloc_format(iloc_op_t op) {
  const char* iloc_code_formats[] = {
    " %s, %s => %s",
    " %s => %s",
    " %s => %s, %s",
    " -> %s",
    " %s -> %s, %s",
    " %s, %s -> %s"
  };
  iloc_code_format_t format = -1;
  switch (op)
  {
  case CBR:
    format = ONE_TO_TWO_JUMP;
    break;
  case JUMP:  case JUMP_I:
    format = ONE_JUMP;
    break;
  case STORE_AI: case STORE_A0:
  case C_STORE_AI: case C_STORE_A0:
    format = ONE_TO_TWO_INST;
    break;
  case CMP_LT: case CMP_LE:
  case CMP_EQ: case CMP_NE:
  case CMP_GT: case CMP_GE:
    format = TWO_TO_ONE_JUMP;
    break;
  case LOAD: case LOAD_I: case C_LOAD:
  case STORE: case C_STORE:
  case I2I:  case C2C:  case C2I:  case I2C:
    format = ONE_TO_ONE_INST;
    break;
  default:
    format = TWO_TO_ONE_INST;
  }
  return iloc_code_formats[format];
}

char* map_arg_type(arg_type_t type, int reg) {
  const char* regs[] = { "rpc", "rfp", "rsp", "rbss" };
  if (reg >= 0) {
    char* c = malloc(10 * sizeof(char));
    if (type == TEMPORARY)
      snprintf(c, 10, "r%d", reg);
    else if (type == LABEL)
      snprintf(c, 10, "l%d", reg);
    else
      snprintf(c, 10, "%d", reg);
    return c;
  } 
  if (type == TEMPORARY && reg >= -4) {
    return regs[reg +4];
  }
  return "";
}

iloc_code_t* create_iloc_code(iloc_op_t op) {
  iloc_code_t* code = malloc(sizeof(iloc_code_t));
  if (code != NULL) {
    code->label = 0;
    code->op = op;
    code->arg_types[0] = NONE;
    code->arg_types[1] = NONE;
    code->arg_types[2] = NONE;
    code->args[0] = 0;
    code->args[1] = 0;
    code->args[2] = 0;
    code->previous = NULL;
    code->next = NULL;
  }
  return code;
}

iloc_code_t* create_iloc_code1op(iloc_op_t op, arg_type_t type, int arg) {
  iloc_code_t* code = create_iloc_code(op);
  if (code != NULL) {
    code->arg_types[0] = type;
    code->args[0] = arg;
  }
  return code;
}
iloc_code_t* create_iloc_code2op(iloc_op_t op, arg_type_t type1, int arg1, arg_type_t type2, int arg2) {
  iloc_code_t* code = create_iloc_code(op);
  if (code != NULL) {
    code->arg_types[0] = type1;
    code->args[0] = arg1;
    code->arg_types[1] = type2;
    code->args[1] = arg2;
  }
  return code;
}
iloc_code_t* create_iloc_code3op(iloc_op_t op, arg_type_t type1, int arg1, arg_type_t type2, int arg2, arg_type_t type3, int arg3) {
  iloc_code_t* code = create_iloc_code(op);
  if (code != NULL) {
    code->arg_types[0] = type1;
    code->args[0] = arg1;
    code->arg_types[1] = type2;
    code->args[1] = arg2;
    code->arg_types[2] = type3;
    code->args[2] = arg3;
  }
  return code;
}

char* iloc_code_to_string(iloc_code_t* code) {
  char* buffer = NULL;
  if (code != NULL) {
    buffer = malloc(ILOC_CODE_BUFFER_SIZE * sizeof(char));
    if (buffer != NULL) {
      int length = 0;
      if (code->label > 0)
        length += snprintf(buffer, ILOC_CODE_BUFFER_SIZE, "l%d: ", code->label);
      length += snprintf(buffer+length, ILOC_CODE_BUFFER_SIZE-length, "%s", map_iloc_op(code->op));
      if (code->op != NOP && code->op != HALT) {
        char* arg0 = map_arg_type(code->arg_types[0], code->args[0]);
        char* arg1 = map_arg_type(code->arg_types[1], code->args[1]);
        char* arg2 = map_arg_type(code->arg_types[2], code->args[2]);
        snprintf(buffer+length, ILOC_CODE_BUFFER_SIZE-length, map_iloc_format(code->op), 
            arg0,
            arg1,
            arg2);
        if (code->args[0] >= 0) free(arg0);
        if (code->args[1] >= 0) free(arg1);
        if (code->args[2] >= 0) free(arg2);
      }
    }
  }
  return buffer;
}

void destroy_iloc_code(iloc_code_t* code) {
  if (code != NULL) {
    code->previous = NULL;
    code->next = NULL;
    free(code);
  }
}

iloc_program_t* create_iloc_program() {
  iloc_program_t* program = malloc(sizeof(iloc_program_t));
  if (program != NULL) {
    program->count = 0;
    program->head = NULL;
    program->tail = NULL;
  }
  return program;
}

iloc_code_t* get_last_iloc_code(iloc_code_t* code) {
  iloc_code_t* last = code;
  while (last->next != NULL) 
    last = last->next;
  return last;
}

iloc_code_t* get_first_iloc_code(iloc_code_t* code) {
  iloc_code_t* first = code;
  while (first->previous != NULL)
    first = first->previous;
  return first;
}

int length_iloc_code(iloc_code_t* code) {
  iloc_code_t* c = get_first_iloc_code(code);
  int count = 1;
  while (c->next != NULL) {
    count++;
    c = c->next;
  }
  return count;
}

void push_iloc_code(iloc_program_t* program, iloc_code_t* code) {
  if (program != NULL && code != NULL) {
    if (program->count == 0) {
      program->head = get_first_iloc_code(code);
      program->tail = get_last_iloc_code(code);
    } else {
      iloc_code_t* first = get_first_iloc_code(code);
      first->previous = program->tail;
      program->tail->next = first;
      program->tail = get_last_iloc_code(code);
    }
    program->count = length_iloc_code(code);
  }
}

void unshift_iloc_code(iloc_program_t* program, iloc_code_t* code) {
  if (program != NULL && code != NULL) {
    if (program->count == 0) {
      program->head = get_first_iloc_code(code);
      program->tail = get_last_iloc_code(code);
    } else {
      iloc_code_t* last = get_last_iloc_code(code);
      last->next = program->head;
      program->head->previous = last;
      program->head = get_first_iloc_code(code);
    }
    program->count = length_iloc_code(code);
  }
}

void empty_iloc_program(iloc_program_t* program) {
  if (program != NULL) {
    program->head = NULL;
    program->tail = NULL;
    program->count = 0;
  }
}

void destroy_iloc_program(iloc_program_t* program) {
  if (program != NULL) {
    iloc_code_t* code = program->head;
    while (code != NULL) {
      iloc_code_t* next = code->next;
      destroy_iloc_code(code);
      code = next;
    }
    free(program);
  }
}

void concat_iloc_program(iloc_program_t* program1, iloc_program_t* program2) {
  if (program1 != NULL && program2 != NULL) {
    push_iloc_code(program1, program2->head);
    empty_iloc_program(program2);
  }
}

int new_label() {
  static int count_label = 1;
  return count_label++;
}

int new_reg() {
  static int count_reg = 0;
  return count_reg++;
}

int rfp_displacement(int disp) {
  static int count_rfp = 0;
  int temp = count_rfp;
  count_rfp += disp;
  return temp;
}

int rbss_displacement(int disp) {
  static int count_rbss = 0;
  int temp = count_rbss;
  count_rbss += disp;
  return temp;
}

void print_program(iloc_program_t* program) {
  if (program != NULL) {
    iloc_code_t* code = program->head;
    while (code != NULL) {
      char* str = iloc_code_to_string(code);
      printf("%s\n", str);
      free(str);
      code = code->next;
    }
  }
}