
#include <stdint.h>
#include <string.h>

#include "debug.h"
#include "elf/output.h"
#include "form/instructions.h"
#include "form/generic.h"
#include "macros.h"
#include "symbols.h"
#include "xmalloc.h"

struct case_t {
    const char *asm;
    uint32_t bytecode;
    size_t p;
};

struct case_t cases[] = {
    { .asm = "mul x1, x2, x3", .bytecode = 0x023100b3 },
    { .asm = "mulh x1, x2, x3", .bytecode = 0x023110b3 },
    { .asm = "mulhsu x1, x2, x3", .bytecode = 0x023120b3 },
    { .asm = "mulhu x1, x2, x3", .bytecode = 0x023130b3 },
    { .asm = "div x1, x2, x3", .bytecode = 0x023140b3 },
    { .asm = "divu x1, x2, x3", .bytecode = 0x023150b3 },
    { .asm = "rem x1, x2, x3", .bytecode = 0x023160b3 },
    { .asm = "remu x1, x2, x3", .bytecode = 0x023170b3 },

    { .asm = "mulw x1, x2, x3", .bytecode = 0x023100bb },
    { .asm = "divw x1, x2, x3", .bytecode = 0x023140bb },
    { .asm = "divuw x1, x2, x3", .bytecode = 0x023150bb },
    { .asm = "remw x1, x2, x3", .bytecode = 0x023160bb },
    { .asm = "remuw x1, x2, x3", .bytecode = 0x023170bb }
};

int test_case(struct case_t c)
{
	const size_t line_sz = strlen(c.asm) + 1;
	char *line = xmalloc(line_sz);
	memcpy(line, c.asm, line_sz);

	char *instruction = strtok(line, " \t");
	char *argstr = strtok(NULL, "");

	const struct formation formation = parse_form(instruction);
	if (!formation.name) {
		logger(ERROR, error_internal,
		       "Unable to find formation for instruction %s in %s",
		       instruction, c.asm);
		return 1;
	}

	struct args args = formation.arg_handler(argstr);

	struct bytecode result = formation.form_handler(
		formation.name, formation.idata, args, c.p);

	if (result.size != sizeof(c.bytecode)) {
		logger(ERROR, error_internal, "invalid size generated for %s",
		       c.asm);
		return 1;
	}
	if (*(uint32_t *)result.data != c.bytecode) {
		logger(ERROR, error_internal,
		       "Expected %.08x but got %.08x while generating %s",
		       c.bytecode, *(uint32_t *)result.data, c.asm);
		return 1;
	}

	return 0;
}

int main(void)
{
	set_exit_loglevel(NODEBUG);
	set_min_loglevel(DEBUG);

	struct symbol_t *start = create_symbol("_start", symbol_label);
	start->section = section_null;
	start->value = 0;

	int errors = 0;

	for (size_t i = 0; i < ARRAY_LENGTH(cases); i++)
		errors += test_case(cases[i]);

	return errors != 0 || get_clean_exit(ERROR);
}