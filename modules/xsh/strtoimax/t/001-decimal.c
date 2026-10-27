/*
 * strtoimax: decimal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtoimax.001
 */
#define IC_IMAX
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ "0", 10, 0, -1 },
	{ "7", 10, 7, -1 },
	{ "42", 10, 42, -1 },
	{ "0042", 10, 42, -1 },
	{ "+17", 10, 17, -1 },
	{ "123", 0, 123, -1 },
};

int main(void)
{
	pc_begin("XSH.strtoimax.001");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
