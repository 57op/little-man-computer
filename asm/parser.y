%{
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "asm/parser_types.h"
#include "parser.tab.h"
#include "lex.yy.h"

void yyerror(yyscan_t scanner, parser_data *data, const char *message);
void add_to_lines(asm_line *line, parser_data *data);
%}

%define api.pure
%define parse.error verbose
%lex-param { void *scanner }
%parse-param { void *scanner } { parser_data *data }

%union {
  asm_opcode opcode;
  char *label;
  uint16_t number;
  asm_line line;
}

%token <opcode> T_ARG_INSTR T_INSTR T_DAT
%token <label> T_LABEL
%token <number> T_NUMBER T_NUMBER_DAT
%token T_NEWLINE

%type <line> unlabeled_instruction instruction
%type <number> number_dat
%%
asm:
  | asm T_NEWLINE
  | asm instruction {
    add_to_lines(&$2, data);
  }
  ;

number_dat:
  T_NUMBER {
    $$ = $1;
  }
  | T_NUMBER_DAT {
    $$ = $1;
  }
  ;

unlabeled_instruction:
  T_ARG_INSTR T_NUMBER {
    $$.opcode = $1;
    $$.num_val = $2;
    $$.label_val = NULL;
  }
  | T_ARG_INSTR T_LABEL {
    $$.opcode = $1;
    $$.label_val = $2;
  }
  | T_DAT number_dat {
    $$.opcode = $1;
    $$.num_val = $2;
    $$.label_val = NULL;
  }
  | T_INSTR {
    $$.opcode = $1;
    $$.num_val = 0;
    $$.label_val = NULL;
  }
  | T_DAT {
    $$.opcode = $1;
    $$.num_val = 0;
    $$.label_val = NULL;
  }
  ;

instruction:
  T_LABEL unlabeled_instruction {
    $$ = $2;
    $$.label = $1;
  }
  | unlabeled_instruction {
    $$ = $1;
    $$.label = NULL;
  }
  ;
%%

void yyerror(yyscan_t scanner, parser_data *data, const char *message) {
  printf("[parser error] %s\n", message);
  exit(1);
}

void add_to_lines(asm_line *line, parser_data *data) {
  if (data->lineno >= 100) {
    yyerror(NULL, data, "too many instructions");
  }

  data->lines[data->lineno++] = *line;
}
