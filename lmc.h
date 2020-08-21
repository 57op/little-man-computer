#ifndef LMC_IMPL_H
#define LMC_IMPL_H

#include <stdint.h>
#include "context.h"

void lmc_init(LitteManComputerContext *ctx);
void lmc_init_with(LitteManComputerContext *ctx, uint16_t *instructions);
void lmc_execute(LitteManComputerContext *ctx);

#endif // LMC_IMPL_H