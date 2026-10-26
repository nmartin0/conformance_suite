/*
 * memccpy() stops after the first copy of c and returns the next byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memccpy.001
 * MUTANT    memccpy.returns_at_c
 * MUTANT    memccpy.no_stop
 * MUTANT    memccpy.stops_before
 */
#include "pc.h"
#include <string.h>

static char src[] = "abcdefcx", dst[32];

/* 1 if p[0..n) are all 'X'. */
static int untouched(const char *p, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		if (p[i] != 'X')
			return 0;
	return 1;
}

int main(void)
{
	void *r;

	pc_begin("XSH.memccpy.001");
	pc_test();
	memset(dst, 'X', sizeof dst);
	r = memccpy(dst, src, 'c', 8);
	pc_check(memcmp(dst, "abc", 3) == 0, "memccpy(c 'c') did not copy \"abc\"");
	pc_check(untouched(dst + 3, sizeof dst - 3), "memccpy(c 'c') wrote past the 'c'");
	pc_check(r == dst + 3, "memccpy(c 'c') returned %s, expected d + 3",
	    r == NULL ? "a null pointer" : "another pointer");
	memset(dst, 'X', sizeof dst);
	r = memccpy(dst, src, 'x', 8);
	pc_check(memcmp(dst, "abcdefcx", 8) == 0, "memccpy(c 'x') did not copy all eight bytes");
	pc_check(untouched(dst + 8, sizeof dst - 8), "memccpy(c 'x') wrote past the eighth byte");
	pc_check(r == dst + 8, "memccpy(c 'x') returned %s, expected d + 8",
	    r == NULL ? "a null pointer" : "another pointer");
	pc_pass();
}
