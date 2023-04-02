#include "types.h"

char* sym_nature_to_string(symbol_nature_t sym_nature) {
  switch(sym_nature) {
    case SYM_UNKNOWN:  return "SYM_UNKNOWN "; break;
    case SYM_LITERAL:  return "SYM_LITERAL "; break;
    case SYM_VARIAVEL: return "SYM_VARIAVEL"; break;
    case SYM_ARRANJO:  return "SYM_ARRANJO "; break;
    case SYM_FUNCAO:   return "SYM_FUNCAO  "; break;
  }
}

char* sym_type_to_string(symbol_type_t sym_type) {
  switch(sym_type) {
    case TYPE_UNDEFINED: return "TYPE_UNDEFINED"; break;
    case TYPE_INT:       return "TYPE_INT      "; break;
    case TYPE_FLOAT:     return "TYPE_FLOAT    "; break;
    case TYPE_CHAR:      return "TYPE_CHAR     "; break;
    case TYPE_BOOL:      return "TYPE_BOOL     "; break;
  }
}