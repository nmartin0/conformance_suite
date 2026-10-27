/*
 * strtoimax: digit limits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtoimax.004
 */
#define IC_IMAX
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
	pc_begin("XSH.strtoimax.004");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
