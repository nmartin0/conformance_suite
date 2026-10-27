/*
 * strtoimax: end pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtoimax.006
 */
#define IC_IMAX
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ "99 bottles", 10, 99, 2 },
	{ "5-3", 10, 5, 1 },
	{ "1_000", 10, 1, 1 },
};

int main(void)
{
	pc_begin("XSH.strtoimax.006");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
