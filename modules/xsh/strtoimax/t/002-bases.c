/*
 * strtoimax: bases.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtoimax.002
 */
#define IC_IMAX
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ "1011", 2, 11, -1 },
	{ "777", 8, 511, -1 },
	{ "ff", 16, 255, -1 },
	{ "FF", 16, 255, -1 },
	{ "0xff", 16, 255, -1 },
	{ "0XfF", 16, 255, -1 },
	{ "zz", 36, 1295, -1 },
	{ "ZZ", 36, 1295, -1 },
	{ "b9", 12, 141, -1 },
};

int main(void)
{
	pc_begin("XSH.strtoimax.002");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
