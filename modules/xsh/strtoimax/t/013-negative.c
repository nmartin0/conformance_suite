/*
 * strtoimax: negative.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtoimax.013
 */
#define IC_IMAX
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ "-17", 10, -17, -1 },
	{ "-0x1f", 16, -31, -1 },
	{ "-017", 0, -15, -1 },
	{ "-101", 2, -5, -1 },
	{ "-0", 10, 0, -1 },
};

int main(void)
{
	pc_begin("XSH.strtoimax.013");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
