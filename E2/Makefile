all: parser scanner main
	gcc parser.tab.o lex.yy.o main.o util.o -o etapa2

main: main.c
	gcc main.c -c  # gera main.o
	gcc util.c -c  # gera util.o

parser: parser.y
	bison -d parser.y
	gcc parser.tab.c -c

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c

clean: 
	rm etapa2 lex.yy.c parser.tab.c parser.tab.h
	rm *.o
