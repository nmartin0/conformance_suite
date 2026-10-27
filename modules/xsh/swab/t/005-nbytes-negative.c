/*
 * swab: nbytes negative.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.swab.005
 */
#include "pc.h"
#include <string.h>
#include <unistd.h>
#include <limits.h>
static void (*volatile f_swab)(const void *restrict, void *restrict, ssize_t) = swab;

int main(void)
{
	char d[6];
	pc_begin("XSH.swab.005");
	pc_test();
	memset(d, 'X', sizeof d);
	f_swab("abcd", d, -2);
	pc_check(memcmp(d, "XXXXXX", 6) == 0, "-2 changed the destination");
	f_swab("abcd", d, -1);
	pc_check(memcmp(d, "XXXXXX", 6) == 0, "-1 changed the destination");
	f_swab("abcd", d, (ssize_t)(-SSIZE_MAX - 1));
	pc_check(memcmp(d, "XXXXXX", 6) == 0, "the most negative value changed the destination");
	pc_pass();
}
