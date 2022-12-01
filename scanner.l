/* Guilherme Sales */

%{

#include "tokens.h"

%}

%option noyywrap
%option yylineno

espaco [ \t\n\r]

%%

"int"espaco { return TK_PR_INT; }
"float"espaco { return TK_PR_FLOAT; }
"bool"espaco { return TK_PR_BOOL; }
"char"espaco { return TK_PR_CHAR; }
"if"espaco { return TK_PR_IF; }
"then"espaco { return TK_PR_THEN; }
"else"espaco { return TK_PR_ELSE; }
"while"espaco { return TK_PR_WHILE; }
"input"espaco { return TK_PR_INPUT; }
"output"espaco { return TK_PR_OUTPUT; }
"return"espaco { return TK_PR_RETURN; }
"for"espaco { return TK_PR_FOR; }

[,;:(){}\+\-*/%^<>=!]espaco { return yytext[0]; }

"<="espaco { return TK_OC_LE; }
">="espaco { return TK_OC_GE; }
"!="espaco { return TK_OC_NE; }
"=="espaco { return TK_OC_EQ; }
"&&"espaco { return TK_OC_AND; }
"||"espaco { return TK_OC_OR; }

[a-zA-Z]espaco { return TK_IDENTIFICADOR; }

[\-+]?[0-9]espaco { return TK_LIT_INT; }
[\-+]?[0-9]\.[0-9]+espaco { return TK_LIT_FLOAT; }

"false"espaco { return TK_LIT_FALSE; }
"true"espaco { return TK_LIT_TRUE; }

'[a-z]'espaco { return TK_LIT_CHAR; }

. { return TK_ERRO; }

%%



