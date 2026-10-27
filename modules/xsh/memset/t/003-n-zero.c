/*
 * memset: n zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memset.003
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memset)(void *, int, size_t) = memset;

int main(void)
{
	char b[4] = "abc";
	pc_begin("XSH.memset.003");
	pc_test();
	pc_check(f_memset(b, 'z', 0) == b, "the return value was not the first argument");
	pc_check(memcmp(b, "abc", 4) == 0, "a fill of 0 bytes changed the array");
	pc_pass();
}
