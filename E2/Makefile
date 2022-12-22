
main: main.c
	gcc main.c -c  # gera main.o
	gcc util.c -c  # gera util.o

parser: parser.y
	bison -d parser.y

scanner: scanner.l
	flex scanner.l

all: parser scanner
	gcc parser.tab.c lex.yy.c main.c util.o -o etapa2

clean: 
	rm etapa2 lex.yy.c parser.tab.c parser.tab.h
	rm *.o
