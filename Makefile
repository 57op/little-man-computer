OBJS = parser.tab.o lex.yy.o asm.o lmc.o main.o
CFLAGS ?=
LIBS ?=

ifdef JIT
	OBJS += lmc_jit.o
	override CFLAGS += -DLMC_USE_JIT
endif

all: | parser lexer main

main: $(OBJS)
	gcc -o main $^ -s -Os $(LIBS)

parser.tab.c: parser

lex.yy.c: lexer

parser:
	bison -d asm/parser.y

lexer:
	flex asm/lexer.l

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@ -s -Os -std=c11 -Wall

clean:
	rm -f *.o main main.exe parser.tab.{c,h} lex.yy.{c,h}