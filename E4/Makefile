all: parser scanner main
	gcc parser.tab.o lex.yy.o main.o valor_lexico.o arvore.o tabela.o -o etapa4

main: main.c
	gcc main.c -c  # gera main.o
	gcc valor_lexico.c -c # gera valor_lexico.o
	gcc arvore.c -c # gera arvore.o
	gcc tabela.c -c # gera tabela.o

parser: parser.y
	bison -d parser.y
	gcc parser.tab.c -c

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c

debug: parser-debug scanner-debug main-debug
	gcc parser.tab.o lex.yy.o main.o valor_lexico.o arvore.o tabela.o -o etapa4-dbg

main-debug: main.c
	gcc main.c -c -g # gera main.o
	gcc valor_lexico.c -c -g # gera valor_lexico.o
	gcc arvore.c -c -g # gera arvore.o
	gcc tabela.c -c -g # gera tabela.o

parser-debug: parser.y
	bison -d parser.y
	gcc parser.tab.c -c -g

scanner-debug: scanner.l
	flex -d scanner.l
	gcc lex.yy.c -c -g

clean: 
	rm etapa* lex.yy.c parser.tab.c parser.tab.h
	rm *.o
