all: parser scanner main
	gcc -g parser.tab.o lex.yy.o main.o util.o utils.o arvore.o -o etapa3

main: main.c
	gcc -g main.c -c  # gera main.o
	gcc -g util.c -c  # gera util.o
	gcc -g utils.c -c # gera utils.o
	gcc -g arvore.c -c # gera arvore.o

parser: parser.y
	bison -d parser.y
	gcc parser.tab.c -c

scanner: scanner.l
	flex scanner.l
	gcc lex.yy.c -c

clean: 
	rm etapa3 lex.yy.c parser.tab.c parser.tab.h
	rm *.o
