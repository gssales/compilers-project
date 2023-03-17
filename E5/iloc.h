#ifndef _ILOC_H_
#define _ILOC_H_

#define ILOC_CODE_BUFFER_SIZE 100

#define ILOC_RBSS -1
#define ILOC_RSP -2
#define ILOC_RFP -3
#define ILOC_RPC -4

typedef enum {
  NOP,
  ADD,  ADD_I,
  SUB,  SUB_I,  R_SUB_I,
  MULT, MULT_I,
  DIV,  DIV_I,  R_DIV_I,
  L_SHIFT,  L_SHIFT_I,
  R_SHIFT,  R_SHIFT_I,
  AND,  AND_I,
  OR, OR_I,
  XOR,  XOR_I,
  LOAD, LOAD_I,
  LOAD_AI, LOAD_A0,
  C_LOAD, C_LOAD_AI,  C_LOAD_A0,
  STORE,  STORE_AI, STORE_A0,
  C_STORE,  C_STORE_AI, C_STORE_A0,
  I2I,  C2C,  C2I,  I2C,
  JUMP_I, JUMP, CBR,
  CMP_LT, CMP_LE,
  CMP_EQ, CMP_NE,
  CMP_GT, CMP_GE
} iloc_op_t;

typedef enum {
  TWO_TO_ONE_INST, /* c0, c1 => c2 */
  ONE_TO_ONE_INST, /* c0 => c1*/
  ONE_TO_TWO_INST, /* c0 => c1, c2 */
  ONE_JUMP,        /* -> l0 */
  ONE_TO_TWO_JUMP, /* c1 -> l1, l2 */
  TWO_TO_ONE_JUMP /* c1, c1 -> l2 */
} iloc_code_format_t;

typedef enum {
  NONE = 0,
  IMMEDIATE = 1,
  TEMPORARY = 2,
  LABEL = 3
} arg_type_t;

typedef struct _iloc_code iloc_code_t;
typedef struct _iloc_code {
  int label;
  iloc_op_t op;
  arg_type_t arg_types[3];
  int args[3];
  iloc_code_t* previous;  // não atribuir esse campo diretamente, apenas pelas funções push_iloc_code unshift_iloc_code
  iloc_code_t* next;      // não atribuir esse campo diretamente, apenas pelas funções push_iloc_code unshift_iloc_code
} iloc_code_t;

iloc_code_t* create_iloc_code(iloc_op_t op);
iloc_code_t* create_iloc_code1op(iloc_op_t op, arg_type_t type, int arg);
iloc_code_t* create_iloc_code2op(iloc_op_t op, arg_type_t type1, int arg1, arg_type_t type2, int arg2);
iloc_code_t* create_iloc_code3op(iloc_op_t op, arg_type_t type1, int arg1, arg_type_t type2, int arg2, arg_type_t type3, int arg3);
char* iloc_code_to_string(iloc_code_t* code);
void destroy_iloc_code(iloc_code_t* code);

typedef struct _iloc_program {
  int count;
  iloc_code_t* head;
  iloc_code_t* tail;
} iloc_program_t;

iloc_program_t* create_iloc_program();
void push_iloc_code(iloc_program_t* program, iloc_code_t* code);
void unshift_iloc_code(iloc_program_t* program, iloc_code_t* code);
void destroy_iloc_program(iloc_program_t* program);

void concat_iloc_program(iloc_program_t* program1, iloc_program_t* program2);

int new_label();
int new_reg();
int rfp_displacement(int disp);
int rbss_displacement(int disp);

void print_program(iloc_program_t* program);

#endif //_ILOC_H_
