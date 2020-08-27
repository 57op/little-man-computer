all: | parser lexer main

main: parser.tab.o lex.yy.o asm.o lmc.o main.o
	gcc -Llibs -o main $^ -s -Os

parser.tab.c: parser

lex.yy.c: lexer

parser:
	bison -d asm/parser.y

lexer:
	flex asm/lexer.l

%.o: %.c
	gcc -c $< -o $@ -s -Os -std=c11 -Wall

clean:
	rm -f *.o main main.exe parser.tab.{c,h} lex.yy.{c,h}