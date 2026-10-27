/*
 * ffs: lowest of several.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ffs.003
 */
#include "pc.h"
#include <strings.h>
#include <limits.h>
static int (*volatile f_ffs)(int) = ffs;
static int (*volatile f_ffsl)(long) = ffsl;
static int (*volatile f_ffsll)(long long) = ffsll;

int main(void)
{
	static const struct { long v; int want; } t[] = {
		{ 6, 2 }, { 0x18, 4 }, { 0xf0, 5 }, { -1, 1 }, { -2, 2 }, { 1, 1 }, { 0x7f00, 9 }, { 12, 3 }
	};
	size_t k;
	pc_begin("XSH.ffs.003");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++) {
		pc_check(f_ffs((int)t[k].v) == t[k].want, "ffs(%ld) was not %d", t[k].v, t[k].want);
		pc_check(f_ffsl(t[k].v) == t[k].want, "ffsl(%ld) was not %d", t[k].v, t[k].want);
		pc_check(f_ffsll(t[k].v) == t[k].want, "ffsll(%ld) was not %d", t[k].v, t[k].want);
	}
	pc_check(f_ffs(INT_MAX) == 1, "ffs(INT_MAX) was not 1");
	pc_check(f_ffs(INT_MAX - 1) == 2, "ffs(INT_MAX - 1) was not 2");
	pc_check(f_ffs(INT_MIN + 8) == 4, "ffs(INT_MIN + 8) was not 4");
	pc_check(f_ffsl(LONG_MAX) == 1, "ffsl(LONG_MAX) was not 1");
	pc_check(f_ffsl(LONG_MIN + 8) == 4, "ffsl(LONG_MIN + 8) was not 4");
	pc_check(f_ffsll(LLONG_MAX) == 1, "ffsll(LLONG_MAX) was not 1");
	pc_check(f_ffsll(LLONG_MIN + 8) == 4, "ffsll(LLONG_MIN + 8) was not 4");
	pc_pass();
}
