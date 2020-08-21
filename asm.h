#ifndef LMC_ASM_H
#define LMC_ASM_H

#include <stdio.h>
#include <stdint.h>

// returns a dynamic allocated vector of instructions
uint16_t *lmc_asm_parse(FILE *fh);

#endif