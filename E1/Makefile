default: main scanner
	gcc lex.yy.o main.o util.o -o etapa1

main: main.c
	gcc main.c -c  # gera main.o
	gcc util.c -c  # gera util.o

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c  # gera lex.yy.o

clean: 
	rm etapa1 lex.yy.c
	rm *.o
