/*
 * swab: even count.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.swab.001
 */
#include "pc.h"
#include <string.h>
#include <unistd.h>
static void (*volatile f_swab)(const void *restrict, void *restrict, ssize_t) = swab;

int main(void)
{
	char d[10];
	pc_begin("XSH.swab.001");
	pc_test();
	memset(d, 'X', sizeof d);
	f_swab("abcdef", d, 6);
	pc_check(memcmp(d, "badcfeXXXX", 10) == 0, "6 bytes were not exchanged pairwise");
	memset(d, 'X', sizeof d);
	f_swab("ab", d, 2);
	pc_check(memcmp(d, "baXXXXXXXX", 10) == 0, "2 bytes were not exchanged");
	pc_pass();
}
