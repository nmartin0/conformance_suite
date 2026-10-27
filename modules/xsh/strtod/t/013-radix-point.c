/*
 * strtod: radix point.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtod.013
 */
#include "fc.h"
#include <string.h>

static const struct fc_case tab[] = {
	{ "1.25", 1.25L, -1 },
	{ "1,25", 1.0L, 1 },
	{ "-3.5", -3.5L, -1 },
};

int main(void)
{
	pc_begin("XSH.strtod.013");
	pc_test();
	fc_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
