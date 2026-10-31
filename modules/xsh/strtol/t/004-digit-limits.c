/*
 * strtol: digit limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtol.004
 * MUTANT    strtol.digit_eq_base_ok
 * MUTANT    strtol.junk_skipped
 * MUTANT    strtol.base16_0x_consumed
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ "1012", 2, 5, 3 },
	{ "789", 8, 7, 1 },
	{ "1fg", 16, 31, 2 },
	{ "12ab", 10, 12, 2 },
	{ "0x", 16, 0, 1 },
};

int main(void)
{
	pc_begin("XSH.strtol.004");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
