#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "lmc.h"

int main(void) {
  LitteManComputerContext lmc;

  lmc_init(&lmc);

  // subtract two numbers
  lmc.mailboxes[0] = 901;
  lmc.mailboxes[1] = 308;
  lmc.mailboxes[2] = 901;
  lmc.mailboxes[3] = 309;
  lmc.mailboxes[4] = 508;
  lmc.mailboxes[5] = 209;
  lmc.mailboxes[6] = 902;
  lmc.mailboxes[7] = 0;

  lmc_execute(&lmc);

  // prepare for executing once again
  lmc_init(&lmc);

  // decrease number to 0 (and print each iteration)
  lmc.mailboxes[0] = 901; // input
  lmc.mailboxes[1] = 902; // output
  lmc.mailboxes[2] = 705; // jump if zero
  lmc.mailboxes[3] = 206; // subtract mailbox 7 from accumulator
  lmc.mailboxes[4] = 601; // jump back to mailbox 1 instruction
  lmc.mailboxes[5] = 000; // halt
  lmc.mailboxes[6] = 1; // store 1 at this memory address

  lmc_execute(&lmc);

  return 0;
}