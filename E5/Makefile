all: parser scanner main
	gcc parser.tab.o lex.yy.o main.o lexvalue.o arvore.o tabela.o stringstack.o inference.o types.o iloc.o -o etapa5

main: main.c
	gcc main.c -c  # gera main.o
	gcc lexvalue.c -c # gera lexvalue.o
	gcc arvore.c -c # gera arvore.o
	gcc tabela.c -c # gera tabela.o
	gcc stringstack.c -c # gera stringstack.o
	gcc inference.c -c # gera inference.o
	gcc types.c -c # gera types.o
	gcc iloc.c -c # gera iloc.o

parser: parser.y
	bison -d parser.y
	gcc parser.tab.c -c

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c

debug: parser-debug scanner-debug main-debug
	gcc parser.tab.o lex.yy.o main.o lexvalue.o arvore.o tabela.o stringstack.o inference.o types.o iloc.o -o etapa5-dbg

main-debug: main.c
	gcc main.c -c -g # gera main.o
	gcc lexvalue.c -c -g # gera lexvalue.o
	gcc arvore.c -c -g # gera arvore.o
	gcc tabela.c -c -g # gera tabela.o
	gcc stringstack.c -c -g # gera stringstack.o
	gcc inference.c -c -g # gera inference.o
	gcc types.c -c -g # gera types.o
	gcc iloc.c -c -g # gera iloc.o

parser-debug: parser.y
	bison -d parser.y
	gcc parser.tab.c -c -g

scanner-debug: scanner.l
	flex -d scanner.l
	gcc lex.yy.c -c -g

clean: 
	rm etapa* lex.yy.c parser.tab.c parser.tab.h
	rm *.o
