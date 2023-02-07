/* Guilherme Sales e Jose Cesar Chagastelles Pinto */

%{

#include "parser.tab.h"

%}

%option noyywrap
%option yylineno

%%

[ \t\n\r]               { /* Ignora espaco, tab, e new line */ }
"int"                   { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_INT, strdup(yytext));
    return TK_PR_INT;
}
"float"                 { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_FLOAT, strdup(yytext));
    return TK_PR_FLOAT;
}
"bool"                  {
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_BOOL, strdup(yytext));
    return TK_PR_BOOL;
}
"char"                  { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_CHAR, strdup(yytext));
    return TK_PR_CHAR;
}
"if"                    { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_IF, strdup(yytext));
    return TK_PR_IF;
}
"then"                  { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_THEN, strdup(yytext));
    return TK_PR_THEN;
}
"else"                  { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_ELSE, strdup(yytext));
    return TK_PR_ELSE;
}
"while"                 { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_WHILE, strdup(yytext));
    return TK_PR_WHILE;
}
"input"                 { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_INPUT, strdup(yytext));
    return TK_PR_INPUT;
}
"output"                { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_OUTPUT, strdup(yytext));
    return TK_PR_OUTPUT;
}
"return"                { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_RETURN, strdup(yytext));
    return TK_PR_RETURN;
}
"for"                   { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_PR_FOR, strdup(yytext));
    return TK_PR_FOR;
}

[\[\],;:(){}\+\-*/%^<>=!]   { 
    yylval.valor_lexico = create_lexvalue(yylineno, 0, strdup(yytext));
    return yytext[0];
}

"<="                    { return TK_OC_LE; }
">="                    { return TK_OC_GE; }
"!="                    { return TK_OC_NE; }
"=="                    { return TK_OC_EQ; }
"&&"                    { return TK_OC_AND; }
"||"                    { return TK_OC_OR; }

"false"                 { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_FALSE, strdup(yytext));
    return TK_LIT_FALSE;
}
"true"                  {
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_TRUE, strdup(yytext));
    return TK_LIT_TRUE;
}

[a-zA-Z]+ { 
  yylval.valor_lexico = create_lexvalue(yylineno, TK_IDENTIFICADOR, strdup(yytext));
  return TK_IDENTIFICADOR;
}
[0-9]+            { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_INT, strdup(yytext));
    return TK_LIT_INT;
}
[0-9]+\.[0-9]+    { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_FLOAT, strdup(yytext));
    return TK_LIT_FLOAT;
}

[0-9]+\.[0-9]+[eE][\-+]?[0-9]+  { /* Notacao cientifica */
                            yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_FLOAT, strdup(yytext));
                            return TK_LIT_FLOAT;
                        }

'.'                     { 
    yylval.valor_lexico = create_lexvalue(yylineno, TK_LIT_CHAR, strdup(yytext));
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
