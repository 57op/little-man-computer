#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "lmc.h"

void lmc_init(LitteManComputerContext *ctx) {
  memset(ctx, 0, sizeof(*ctx));
}

void lmc_init_with(LitteManComputerContext *ctx, uint16_t *instructions) {
  ctx->accumulator = 0;
  ctx->program_counter = 0;
  ctx->flags = 0;
  ctx->executing = false;
  memcpy(ctx->mailboxes, instructions, sizeof(*instructions) * 100);
}

static void lmc_placeholder_instruction(LitteManComputerContext *ctx, uint16_t data) {}

static void lmc_halt_instruction(LitteManComputerContext *ctx, uint16_t data) {
  ctx->executing = false;
}

static void lmc_add_instruction(LitteManComputerContext *ctx, uint16_t mailbox) {
  ctx->accumulator += ctx->mailboxes[mailbox];
  // unset negative flag
  // Similarly to SUBTRACT, one could set the negative flag on overflow.
  ctx->flags &= ~LMC_NEGATIVE_FLAG;
}

static void lmc_subtract_instruction(LitteManComputerContext *ctx, uint16_t mailbox) {
  uint16_t value = ctx->mailboxes[mailbox];

  if (value > ctx->accumulator) {
    ctx->flags |= LMC_NEGATIVE_FLAG;
  } else {
    ctx->flags &= ~LMC_NEGATIVE_FLAG;
  }

  ctx->accumulator -= value;
}

static void lmc_store_instruction(LitteManComputerContext *ctx, uint16_t mailbox) {
  ctx->mailboxes[mailbox] = ctx->accumulator;
}

static void lmc_load_instruction(LitteManComputerContext *ctx, uint16_t mailbox) {
  ctx->accumulator = ctx->mailboxes[mailbox];
}

static void lmc_branch_instruction(LitteManComputerContext *ctx, uint16_t address) {
  ctx->program_counter = address;
}

static void lmc_branch_if_zero_instruction(LitteManComputerContext *ctx, uint16_t address) {
  if (ctx->accumulator == 0 && (ctx->flags & LMC_NEGATIVE_FLAG) == 0) {
    ctx->program_counter = address;
  }
}

static void lmc_branch_if_positive_instruction(LitteManComputerContext *ctx, uint16_t address) {
  if ((ctx->flags & LMC_NEGATIVE_FLAG) == 0) {
    ctx->program_counter = address;
  }
}

static void lmc_input_instruction(LitteManComputerContext *ctx) {
  scanf("%" SCNd16, &ctx->accumulator);
}

static void lmc_output_instruction(LitteManComputerContext *ctx) {
  printf("%" PRId16 "\n", ctx->accumulator);
}

static void lmc_input_output_instruction(LitteManComputerContext *ctx, uint16_t op) {
  static void (*instructions[])(LitteManComputerContext *ctx) = {
    lmc_input_instruction,
    lmc_output_instruction
  };

  instructions[(op - 1) % 2](ctx);
}

static void (*lmc_instructions[])(LitteManComputerContext *ctx, uint16_t data) = {
  lmc_halt_instruction, // 0xx
  lmc_add_instruction, // 1xx
  lmc_subtract_instruction, // 2xx
  lmc_store_instruction, // 3xx
  lmc_placeholder_instruction,
  lmc_load_instruction, // 5xx
  lmc_branch_instruction, // 6xx
  lmc_branch_if_zero_instruction, // 7xx,
  lmc_branch_if_positive_instruction, // 8xx
  lmc_input_output_instruction, // 9xx
};

void lmc_execute(LitteManComputerContext *ctx) {
  ctx->executing = true;

  while (ctx->executing) {
    // fetch instruction
    uint16_t instruction = ctx->mailboxes[ctx->program_counter] % 1000; // 0 to 999
    size_t op_code = instruction / 100; // first digit
    uint16_t data = instruction % 100; // last two digits

    // increase program counter
    ctx->program_counter += 1;

    // execute instruction
    lmc_instructions[op_code](ctx, data);
  }
}