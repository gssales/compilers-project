typedef union TokenValue {
  int i;
  float f;
  char c;
} token_value_t;

typedef struct LexValue {
  int line_number;
  token_value_t tk_value;
  int tk_type;
} value_t;

value_t* create_lexvalue(int lineno, int token, char* lexema);

void print_lexvalue(value_t* lexvalue);