all: parser scanner main
	gcc parser.tab.o lex.yy.o main.o lexvalue.o arvore.o tabela.o stringlist.o inference.o types.o iloc.o asm.o cfg.o -o etapa7

main: main.c
	gcc main.c -c  # gera main.o
	gcc lexvalue.c -c # gera lexvalue.o
	gcc arvore.c -c # gera arvore.o
	gcc tabela.c -c # gera tabela.o
	gcc stringlist.c -c # gera stringlist.o
	gcc inference.c -c # gera inference.o
	gcc types.c -c # gera types.o
	gcc iloc.c -c # gera iloc.o
	gcc asm.c -c # gera asm.o
	gcc cfg.c -c # gera cfg.o

parser: parser.y
	bison -d parser.y
	gcc parser.tab.c -c

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c

debug: parser-debug scanner-debug main-debug
	gcc parser.tab.o lex.yy.o main.o lexvalue.o arvore.o tabela.o stringlist.o inference.o types.o iloc.o asm.o cfg.o -o etapa7-dbg

main-debug: main.c
	gcc main.c -c -g # gera main.o
	gcc lexvalue.c -c -g # gera lexvalue.o
	gcc arvore.c -c -g # gera arvore.o
	gcc tabela.c -c -g # gera tabela.o
	gcc stringlist.c -c -g # gera stringlist.o
	gcc inference.c -c -g # gera inference.o
	gcc types.c -c -g # gera types.o
	gcc iloc.c -c -g # gera iloc.o
	gcc asm.c -c -g # gera asm.o
	gcc cfg.c -c -g # gera cfg.o

parser-debug: parser.y
	bison -d parser.y
	gcc parser.tab.c -c -g

scanner-debug: scanner.l
	flex -d scanner.l
	gcc lex.yy.c -c -g

clean: 
	rm etapa* lex.yy.c parser.tab.c parser.tab.h
	rm *.o

package:
	tar cvzf etapa7.tgz .
