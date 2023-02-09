/* Guilherme Sales e Jose Cesar Chagastelles Pinto */

%{

#include "parser.tab.h"

%}

%option noyywrap
%option yylineno

%%

[ \t\n\r]               { /* Ignora espaco, tab, e new line */ }
"int"                   { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_INT, dup);
    free(dup);
    return TK_PR_INT;
}
"float"                 { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_FLOAT, dup);
    free(dup);
    return TK_PR_FLOAT;
}
"bool"                  {
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_BOOL, dup);
    free(dup);
    return TK_PR_BOOL;
}
"char"                  { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_CHAR, dup);
    free(dup);
    return TK_PR_CHAR;
}
"if"                    { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_IF, dup);
    free(dup);
    return TK_PR_IF;
}
"then"                  { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_THEN, dup);
    free(dup);
    return TK_PR_THEN;
}
"else"                  { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_ELSE, dup);
    free(dup);
    return TK_PR_ELSE;
}
"while"                 { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_WHILE, dup);
    free(dup);
    return TK_PR_WHILE;
}
"input"                 { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_INPUT, dup);
    free(dup);
    return TK_PR_INPUT;
}
"output"                { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_OUTPUT, dup);
    free(dup);
    return TK_PR_OUTPUT;
}
"return"                { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_RETURN, dup);
    free(dup);
    return TK_PR_RETURN;
}
"for"                   { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_FOR, dup);
    free(dup);
    return TK_PR_FOR;
}

[\[\],;:(){}\+\-*/%^<>=!]   { 
    char* dup = strdup(yytext);
    //yylval.valor_lexico = create_lexvalue(yylineno, 0, dup);
    free(dup);
    return yytext[0];
}

"<="                    { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_OC_LE, dup);
    free(dup);
    return TK_OC_LE;
}
">="                    { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_OC_GE, dup);
    free(dup);
    return TK_OC_GE; 
}
"!="                    { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_OC_NE, dup);
    free(dup);
    return TK_OC_NE;
}
"=="                    { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_OC_EQ, dup);
    free(dup);
    return TK_OC_EQ;
}
"&&"                    { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_OC_AND, dup);
    free(dup);
    return TK_OC_AND;
}
"||"                    { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_OC_OR, dup);
    free(dup);
    return TK_OC_OR;
}

"false"                 { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_FALSE, dup);
    free(dup);
    return TK_LIT_FALSE;
}
"true"                  {
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_TRUE, dup);
    free(dup);
    return TK_LIT_TRUE;
}

[a-zA-Z]+               { 
    char* dup = strdup(yytext);
  yylval.valor_lexico = create_lexvalue(yylineno, TK_IDENTIFICADOR, dup);
    free(dup);
  return TK_IDENTIFICADOR;
}
[0-9]+                  { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_INT, dup);
    free(dup);
    return TK_LIT_INT;
}
[0-9]+\.[0-9]+          { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_FLOAT, dup);
    free(dup);
    return TK_LIT_FLOAT;
}

[0-9]+\.[0-9]+[eE][\-+]?[0-9]+  { /* Notacao cientifica */
    char* dup = strdup(yytext);
                            yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_FLOAT, dup);
    free(dup);
                            return TK_LIT_FLOAT;
                        }

'.'                     { 
    char* dup = strdup(yytext);
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_CHAR, dup);
    free(dup);
    return TK_LIT_CHAR; }

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
