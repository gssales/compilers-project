/* Guilherme Sales e Jose Cesar Chagastelles Pinto */

%{

#include "parser.tab.h"

%}

%option noyywrap
%option yylineno

%%

[ \t\n\r]               { /* Ignora espaco, tab, e new line */ }
"int"                   { return TK_PR_INT; }
"float"                 { return TK_PR_FLOAT; }
"bool"                  { return TK_PR_BOOL; }
"char"                  { return TK_PR_CHAR; }
"if"                    { return TK_PR_IF; }
"then"                  { return TK_PR_THEN; }
"else"                  { return TK_PR_ELSE; }
"while"                 { return TK_PR_WHILE; }
"input"                 { return TK_PR_INPUT; }
"output"                { return TK_PR_OUTPUT; }
"return"                { return TK_PR_RETURN; }
"for"                   { return TK_PR_FOR; }

[\[\],;:(){}\+\-*/%^<>=!]   { return yytext[0]; }

"<="                    { return TK_OC_LE; }
">="                    { return TK_OC_GE; }
"!="                    { return TK_OC_NE; }
"=="                    { return TK_OC_EQ; }
"&&"                    { return TK_OC_AND; }
"||"                    { return TK_OC_OR; }

"false"                 { return TK_LIT_FALSE; }
"true"                  { return TK_LIT_TRUE; }

[a-zA-Z]+               { return TK_IDENTIFICADOR; }
[0-9]+            { return TK_LIT_INT; }
[0-9]+\.[0-9]+    { return TK_LIT_FLOAT; }

[0-9]+\.[0-9]+[eE][\-+]?[0-9]+  { /* Notacao cientifica */
                            return TK_LIT_FLOAT;
                        }

'.'                     { return TK_LIT_CHAR; }

\/\*([^*]|\*+[^*/])*\*+\/ { /* Ignora comentarios multi linha 
                            ([^*]|\*+[^*\/])*  -> reconhece qualquer sequencia que não contenha um * seguido de /
                                [^*] -> reconhece qqr char que não seja um *
                                \*+[^*\/] -> reconhece uma sequencia de * contanto que não seja seguido de /
                        */}
\/\/.*                  { /* Ignora comentarios de linhas simples */ }

''                      { /* Char vazio */
                            return TK_ERRO;
                        }
'[^'\n][^'\n][^'\n]*'   { /* Char com mais de 1 caractere */
                            return TK_ERRO;
                        }
'[^'\n]*                { /* Char que nao fecha aspas simples */ 
                            return TK_ERRO;
                        }

.                       { return TK_ERRO; }

%%
