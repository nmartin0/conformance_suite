/*
 * strnlen() returns the smaller of the length and maxlen.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strnlen.002
 * MUTANT    strnlen.ignores_max
 * MUTANT    strnlen.off_by_one
 */
#include "pc.h"
#include <string.h>

static char s[] = "abcdef", empty[8];	/* at least maxlen bytes: see XSH.strnlen.005 */

int main(void)
{
	static const size_t m[4] = { 10, 6, 3, 0 }, want[4] = { 6, 6, 3, 0 };
	size_t r[5];
	int i;

	pc_begin("XSH.strnlen.002");
	pc_test();
	for (i = 0; i < 4; i++)
		r[i] = strnlen(s, m[i]);
	r[4] = strnlen(empty, 5);
	for (i = 0; i < 4; i++)
		pc_check(r[i] == want[i], "strnlen(\"abcdef\", %zu) returned %zu, expected %zu", m[i], r[i], want[i]);
	pc_check(r[4] == 0, "strnlen(\"\", 5) returned %zu", r[4]);
	pc_pass();
}
