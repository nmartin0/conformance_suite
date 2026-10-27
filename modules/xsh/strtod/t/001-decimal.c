/*
 * strtod: decimal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.001
 */
#include "fc.h"
#include <string.h>

static const struct fc_case tab[] = {
	{ "1.5", 1.5L, -1 },
	{ "100", 100.0L, -1 },
	{ ".5", 0.5L, -1 },
	{ "5.", 5.0L, -1 },
	{ "0.25e1", 2.5L, -1 },
	{ "125e-2", 1.25L, -1 },
	{ "2E+3", 2000.0L, -1 },
	{ "0", 0.0L, -1 },
};

int main(void)
{
	pc_begin("XSH.strtod.001");
	pc_test();
	fc_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
