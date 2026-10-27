/*
 * strtoimax: base zero u.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtoimax.016
 */
#define IC_UMAX
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
	pc_begin("XSH.strtoimax.016");
	pc_test();
	ic_run(tab, (int)(sizeof tab / sizeof tab[0]));
	pc_pass();
}
