all: parser.tab.o lex.yy.o asm.o lmc.o main.o
	gcc -Llibs -o main $^ -s -Os

parser.tab.c parser.tab.h lex.yy.c lex.yy.h:
	bison -d asm/parser.y
	flex asm/lexer.l

%.o: %.c
	gcc -c $< -o $@ -s -Os -std=c11 -Wall

clean:
	rm -f *.o main main.exe parser.tab.{c,h} lex.yy.{c,h}