/*
 * ffs: single bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ffs.002
 */
#include "pc.h"
#include <strings.h>
#include <limits.h>
static int (*volatile f_ffs)(int) = ffs;
static int (*volatile f_ffsl)(long) = ffsl;
static int (*volatile f_ffsll)(long long) = ffsll;

static int vbits_int(void) { int n = 0; unsigned long long m = INT_MAX; while (m) { n++; m >>= 1; } return n; }
static int vbits_long(void) { int n = 0; unsigned long long m = LONG_MAX; while (m) { n++; m >>= 1; } return n; }
static int vbits_ll(void) { int n = 0; unsigned long long m = LLONG_MAX; while (m) { n++; m >>= 1; } return n; }

int main(void)
{
	int k, nb;
	pc_begin("XSH.ffs.002");
	pc_test();
	nb = vbits_int();
	for (k = 0; k < nb; k++)
		pc_check(f_ffs((int)(1u << k)) == k + 1, "ffs(1 << %d) was not %d", k, k + 1);
	pc_check(f_ffs(INT_MIN) == nb + 1, "ffs(INT_MIN) was not %d", nb + 1);
	nb = vbits_long();
	for (k = 0; k < nb; k++)
		pc_check(f_ffsl((long)(1ul << k)) == k + 1, "ffsl(1 << %d) was not %d", k, k + 1);
	pc_check(f_ffsl(LONG_MIN) == nb + 1, "ffsl(LONG_MIN) was not %d", nb + 1);
	nb = vbits_ll();
	for (k = 0; k < nb; k++)
		pc_check(f_ffsll((long long)(1ull << k)) == k + 1, "ffsll(1 << %d) was not %d", k, k + 1);
	pc_check(f_ffsll(LLONG_MIN) == nb + 1, "ffsll(LLONG_MIN) was not %d", nb + 1);
	pc_pass();
}
