
main: main.c
	gcc main.c -c  # gera main.o
	gcc util.c -c  # gera util.o

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c  # gera lex.yy.o

all: main scanner
	gcc lex.yy.o main.o util.o -o etapa1

clean: 
	rm etapa1 lex.yy.c
	rm *.o
