%option noyywrap
%option reentrant
%option bison-bridge
%option header-file="lex.yy.h"

%top {
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
}

%{
#include <string.h>

#include "asm/parser_types.h"
#include "parser.tab.h"
%}

%%
ADD         { yylval->opcode = LMC_ADD; return T_ARG_INSTR; }
SUB         { yylval->opcode = LMC_SUB; return T_ARG_INSTR; }
STA         { yylval->opcode = LMC_STA; return T_ARG_INSTR; }
LDA         { yylval->opcode = LMC_LDA; return T_ARG_INSTR; }
BRA         { yylval->opcode = LMC_BRA; return T_ARG_INSTR; }
BRZ         { yylval->opcode = LMC_BRZ; return T_ARG_INSTR; }
BRP         { yylval->opcode = LMC_BRP; return T_ARG_INSTR; }
INP         { yylval->opcode = LMC_INP; return T_INSTR; }
OUT         { yylval->opcode = LMC_OUT; return T_INSTR; }
HLT|COB     { yylval->opcode = LMC_HLT; return T_INSTR; }
DAT         { yylval->opcode = LMC_DAT; return T_DAT; }
[A-Z]+      { yylval->label = strdup(yytext); return T_LABEL; }
[0-9]{1,2}  { yylval->number = atoi(yytext); return T_NUMBER; }
[0-9]{3}    { yylval->number_dat = atoi(yytext); return T_NUMBER_DAT; }
\n          { return T_NEWLINE; }
\/\/.*        ;
.           ;
%%
