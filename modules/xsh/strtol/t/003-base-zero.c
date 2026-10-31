/*
 * strtol: base zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtol.003
 * MUTANT    strtol.base0_octal_as_decimal
 * MUTANT    strtol.base0_no_hex
 * MUTANT    strtol.base0_0x_consumed
 * MUTANT    strtol.base0_octal_accepts_89
 * MUTANT    strtol.end_at_start
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ "0x1F", 0, 31, -1 },
	{ "0X1f", 0, 31, -1 },
	{ "017", 0, 15, -1 },
	{ "0", 0, 0, -1 },
	{ "10", 0, 10, -1 },
	{ "08", 0, 0, 1 },
	{ "0x", 0, 0, 1 },
	{ "0xg", 0, 0, 1 },
};

int main(void)
{
	pc_begin("XSH.strtol.003");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
