#include <jit/jit.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "lmc_jit.h"

static uint16_t lmc_scanf(void) {
  uint16_t n;
  scanf("%" SCNu16, &n);
  return n;
}

static void lmc_printf(uint16_t n) {
  printf("%" PRIu16 "\n", n);
}

/*
 * for simple debugging
static void jit_insn_puts(jit_function_t fn, const char *msg) {
  jit_type_t params[] = { jit_type_void_ptr };
  jit_value_t msgptr = jit_value_create_long_constant(
    fn, jit_type_void_ptr, (long) msg);

  jit_type_t signature = jit_type_create_signature(
    jit_abi_cdecl, jit_type_int, params, 1, 1);
  jit_value_t values[] = { msgptr };

  jit_insn_call_native(fn, "puts", puts, signature, values, 1, JIT_CALL_NOTHROW);
}

static void jit_insn_putn(jit_function_t fn, jit_value_t n) {
  jit_type_t params[] = { jit_type_ushort };
  jit_type_t signature = jit_type_create_signature(
    jit_abi_cdecl, jit_type_void, params, 1, 1);
  jit_value_t values[] = { n };

  jit_insn_call_native(fn, "lmc_printf", lmc_printf, signature, values, 1, JIT_CALL_NOTHROW);
}
*/

static void jit_lmc_placeholder_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t data) {}

static void jit_lmc_halt_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t data) {
  jit_insn_branch(function, &labels[100]);
}

static void jit_lmc_add_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t mailbox) {
  jit_value_t mailboxes = context[0];
  jit_value_t accumulator = context[1];
  jit_value_t value = jit_insn_load_relative(
    function, mailboxes, mailbox * sizeof(uint16_t), jit_type_ushort);

  jit_value_t result = jit_insn_add(function, accumulator, value);
  jit_insn_store(function, accumulator, result);
}

static void jit_lmc_sub_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t mailbox) {
  jit_value_t mailboxes = context[0];
  jit_value_t accumulator = context[1];
  jit_value_t flags = context[2];
  jit_value_t value = jit_insn_load_relative(
    function, mailboxes, mailbox * sizeof(uint16_t), jit_type_ushort);

  // if value > accumulator set negative flag
  // else unset negative flag
  jit_value_t flag_temp = jit_insn_gt(function, value, accumulator);
  jit_value_t acc_temp = jit_insn_sub(function, accumulator, value);

  jit_insn_store(function, accumulator, acc_temp);
  jit_insn_store(function, flags, flag_temp);
}

static void jit_lmc_store_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t mailbox) {
  jit_value_t mailboxes = context[0];
  jit_value_t accumulator = context[1];

  jit_insn_store_relative(
    function, mailboxes, mailbox * sizeof(uint16_t), accumulator);
}

static void jit_lmc_load_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t mailbox) {
  jit_value_t mailboxes = context[0];
  jit_value_t accumulator = context[1];
  jit_value_t value = jit_insn_load_relative(
    function, mailboxes, mailbox * sizeof(uint16_t), jit_type_ushort);

  jit_insn_store(function, accumulator, value);
}

static void jit_lmc_branch_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t label_index) {
  jit_insn_branch(function, &labels[label_index]);
}

static void jit_lmc_branch_zero_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t label_index) {
  jit_value_t accumulator = context[1];
  jit_value_t zero = jit_value_create_nint_constant(function, jit_type_ushort, 0);
  jit_value_t is_zero = jit_insn_eq(function, accumulator, zero);

  jit_insn_branch_if(function, is_zero, &labels[label_index]);
}

static void jit_lmc_branch_positive_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t label_index) {
  jit_value_t flags = context[2];
  jit_value_t is_positive = jit_insn_eq(function, flags, jit_value_create_nint_constant(function, jit_type_ubyte, 0));

  jit_insn_branch_if(function, is_positive, &labels[label_index]);
}

static void jit_lmc_input_output_instruction(jit_function_t function, jit_value_t *context, jit_label_t *labels, uint8_t op) {
  jit_value_t accumulator = context[1];

  if (op == 1) {
    // scanf signature
    jit_type_t signature = jit_type_create_signature(
      jit_abi_cdecl, jit_type_ushort, NULL, 0, 1);
    jit_value_t n = jit_insn_call_native(function, "lmc_scanf", lmc_scanf, signature, NULL, 0, JIT_CALL_NOTHROW);
    jit_type_free(signature);

    // set accumulator
    jit_insn_store(function, accumulator, n);
  } else if (op == 2) {
    // printf signature
    jit_type_t params[] = { jit_type_ushort };
    jit_type_t signature = jit_type_create_signature(
      jit_abi_cdecl, jit_type_void, params, 1, 1);
    jit_value_t args[] = { accumulator };

    // call
    jit_insn_call_native(function, "lmc_printf", lmc_printf, signature, args, 1, JIT_CALL_NOTHROW);
    jit_type_free(signature);
  }
}

static void (*jit_lmc_instructions[])(jit_function_t, jit_value_t *, jit_label_t *, uint8_t) = {
  jit_lmc_halt_instruction, // 0xx
  jit_lmc_add_instruction, // 1xx
  jit_lmc_sub_instruction, // 2xx
  jit_lmc_store_instruction, // 3xx
  jit_lmc_placeholder_instruction,
  jit_lmc_load_instruction, // 5xx
  jit_lmc_branch_instruction, // 6xx, branch
  jit_lmc_branch_zero_instruction, // 7xx, branch if zero
  jit_lmc_branch_positive_instruction, // 8xx, branch if positive
  jit_lmc_input_output_instruction, // 9xx
};

void lmc_execute_jit(LitteManComputerContext *ctx) {
  jit_context_t jit_context = jit_context_create();
  jit_context_build_start(jit_context);

  // function signature
  jit_type_t jit_type_ushort_ptr = jit_type_create_pointer(jit_type_ushort, 1);
  jit_type_t parameters[] = {
    jit_type_ushort_ptr, // mailboxes
  };

  jit_type_t signature = jit_type_create_signature(
    jit_abi_cdecl, jit_type_void, parameters, 1, 1);

	jit_function_t function = jit_function_create(jit_context, signature);

	jit_type_free(signature);

  // function body
  // constants
  jit_value_t c_0 = jit_value_create_nint_constant(
    function, jit_type_ubyte, 0);

  // values
  jit_value_t mailboxes = jit_value_get_param(function, 0);
  jit_value_t accumulator = jit_value_create(function, jit_type_ushort);
  jit_value_t flags = jit_value_create(function, jit_type_ubyte);

  jit_insn_store(function, accumulator, c_0);
  jit_insn_store(function, flags, c_0);

  jit_label_t labels[101];
  jit_value_t context[] = { mailboxes, accumulator, flags };

  // initialize labels
  for (size_t i = 0; i < 101; i++) {
    labels[i] = jit_label_undefined;
  }

  for (size_t i = 0; i < 100; i++) {
    uint16_t instruction = ctx->mailboxes[i] % 1000; // 0 to 999
    uint8_t op_code = instruction / 100; // first digit
    uint8_t data = instruction % 100; // last two digits

    // generate code
    jit_insn_label(function, &labels[i]);
    jit_lmc_instructions[op_code](function, context, labels, data);
  }

  jit_insn_label(function, &labels[100]);
  // /function

  // compile function
  jit_function_compile(function);
	jit_context_build_end(jit_context);

	// run
  {
    jit_ptr mailboxes = &ctx->mailboxes[0];

    void *args[] = {
      &mailboxes
    };
	  jit_function_apply(function, args, NULL);
  }

	jit_context_destroy(jit_context);
}