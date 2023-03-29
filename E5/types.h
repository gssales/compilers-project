#ifndef _TYPES_H_
#define _TYPES_H_

typedef enum {
  AST_NONE,
	AST_FUNCTION, 
	AST_COMMAND, 
	AST_EXPRESSION, 
	AST_VAR_INITIALIZED, 
	AST_LIT_DECLARED
} ast_type_t;

typedef enum { 
  SYM_UNKNOWN,
  SYM_LITERAL,
  SYM_VARIAVEL,
  SYM_ARRANJO,
  SYM_FUNCAO
} symbol_nature_t ;

typedef enum {
  TYPE_UNDEFINED,
  TYPE_FLOAT,
  TYPE_INT,
  TYPE_BOOL,
  TYPE_CHAR
} symbol_type_t;

char* sym_nature_to_string(symbol_nature_t sym_nature);
char* sym_type_to_string(symbol_type_t sym_type);

#endif //_TYPES_H_