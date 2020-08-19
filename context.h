#ifndef LMC_CONTEXT_H
#define LMC_CONTEXT_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
  LMC_NEGATIVE_FLAG = 1
} LitteManComputerFlags;

typedef struct {
  uint16_t mailboxes[100]; // each mailbox holds [0, 999] <-> 1000 ~ 10 bits
  uint16_t accumulator;
  uint8_t program_counter; // [0, 99] <-> 100 ~ 6 bits
  uint8_t flags; // can hold multiple flags

  bool executing;
} LitteManComputerContext;

#endif // LMC_CONTEXT_H