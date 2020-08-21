#ifndef ASM_TYPES_H
#define ASM_TYPES_H

#include <stdlib.h>
#include <stdint.h>

typedef enum {
  LMC_ADD, // 0
  LMC_SUB, // 1
  LMC_STA, // 2
  LMC_LDA, // 3
  LMC_BRA, // 4
  LMC_BRZ, // 5
  LMC_BRP, // 6
  LMC_INP, // 7
  LMC_OUT, // 8
  LMC_HLT, // 9
  LMC_DAT, // 10
} asm_opcode;

typedef struct {
  char *label;
  asm_opcode opcode;
  char *label_val;
  uint16_t num_val;
} asm_line;

typedef struct {
  asm_line lines[100];
  size_t lineno;
} parser_data;

#endif