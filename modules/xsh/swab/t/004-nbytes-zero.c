/*
 * swab: nbytes zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.swab.004
 */
#include "pc.h"
#include <string.h>
#include <unistd.h>
static void (*volatile f_swab)(const void *restrict, void *restrict, ssize_t) = swab;

int main(void)
{
	char d[6];
	pc_begin("XSH.swab.004");
	pc_test();
	memset(d, 'X', sizeof d);
	f_swab("abcd", d, 0);
	pc_check(memcmp(d, "XXXXXX", 6) == 0, "the destination changed");
	pc_pass();
}
