#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "asm.h"
#include "lmc.h"

#ifdef LMC_USE_JIT
#include "lmc_jit.h"
#endif

int main(int argc, const char *argv[]) {
  #ifdef LMC_USE_JIT
  assert(argc <= 3);
  #else
  assert(argc == 2);
  #endif

  FILE *fh = fopen(argv[1], "r");

  if (fh == NULL) {
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    return 1;
  }

  uint16_t *instructions = lmc_asm_parse(fh);
  LitteManComputerContext lmc;

  lmc_init_with(&lmc, instructions);

  #ifdef LMC_USE_JIT
  bool use_jit = argc == 3;

  if (use_jit) {
    lmc_execute_jit(&lmc);
  } else {
    lmc_execute(&lmc);
  }
  #else
  lmc_execute(&lmc);
  #endif

  free(instructions);
  fclose(fh);

  return 0;
}