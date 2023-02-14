all: parser scanner main
	gcc parser.tab.o lex.yy.o main.o util.o utils.o arvore.o -o etapa3

main: main.c
	gcc main.c -c  # gera main.o
	gcc util.c -c  # gera util.o
	gcc utils.c -c # gera utils.o
	gcc arvore.c -c # gera arvore.o

parser: parser.y
	bison -d parser.y
	gcc parser.tab.c -c

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c

clean: 
	rm etapa3 lex.yy.c parser.tab.c parser.tab.h
	rm *.o
