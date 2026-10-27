/*
 * swab: odd count.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.swab.006
 */
#include "pc.h"
#include <string.h>
#include <unistd.h>
static void (*volatile f_swab)(const void *restrict, void *restrict, ssize_t) = swab;

int main(void)
{
	char d[10];
	pc_begin("XSH.swab.006");
	pc_test();
	memset(d, 'X', sizeof d);
	f_swab("abcde", d, 5);
	pc_check(memcmp(d, "badc", 4) == 0, "the first 4 bytes were not exchanged pairwise");
	memset(d, 'X', sizeof d);
	f_swab("abcdefg", d, 7);
	pc_check(memcmp(d, "badcfe", 6) == 0, "the first 6 bytes were not exchanged pairwise");
	pc_pass();
}
