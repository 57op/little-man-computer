#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "asm/parser_types.h"
#include "parser.tab.h"
#include "lex.yy.h"

static void asm_resolve_labels(asm_line *lines, size_t len) {
  for (size_t i = 0; i < len; i++) {
    asm_line *line = &lines[i];

    if (line->label_val == NULL) {
      continue;
    }

    for (size_t j = 0; j < len; j++) {
      if (lines[j].label != NULL && strcmp(line->label_val, lines[j].label) == 0) {
        line->num_val = j;
        break;
      }
    }
  }
}

uint16_t *lmc_asm_parse(FILE *fh) {
  yyscan_t scanner;
  parser_data data = { 0 };

  yylex_init(&scanner);
  yyset_in(fh, scanner);
  yyparse(scanner, &data);
  yylex_destroy(scanner);

  asm_resolve_labels(data.lines, data.lineno);

  // translate to machine code
  // free (strdup) memory
  uint16_t *instructions;
  size_t instructions_size = sizeof(*instructions) * 100;

  if ((instructions = malloc(instructions_size)) == NULL) {
    fprintf(stderr, "cannot allocate memory\n");
    exit(1);
  }

  memset(instructions, 0, instructions_size);

  for (size_t i = 0; i < data.lineno; i++) {
    asm_line *line = &data.lines[i];

    //printf("[%ld] %s %d (%s, %d)\n", i, line->label, line->opcode, line->label_val, line->num_val);

    if (line->label != NULL) {
      free(line->label);
    }

    if (line->label_val != NULL) {
      free(line->label_val);
    }

    uint16_t instruction;

    switch (line->opcode) {
      case LMC_ADD:
        instruction = 100;
        break;

      case LMC_SUB:
        instruction = 200;
        break;

      case LMC_STA:
        instruction = 300;
        break;

      case LMC_LDA:
        instruction = 500;
        break;

      case LMC_BRA:
        instruction = 600;
        break;

      case LMC_BRZ:
        instruction = 700;
        break;

      case LMC_BRP:
        instruction = 800;
        break;

      case LMC_INP:
        instruction = 901;
        break;

      case LMC_OUT:
        instruction = 902;
        break;

      case LMC_HLT:
      case LMC_DAT:
      default:
        instruction = 0;
        break;
    }

    instructions[i] = instruction + line->num_val;
  }

  return instructions;
}