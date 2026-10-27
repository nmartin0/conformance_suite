/*
 * ffs: beyond int.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ffs.004
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
	pc_begin("XSH.ffs.004");
	pc_test();
	pc_check(f_ffsll(1LL << 32) == 33, "ffsll(2^32) was not 33");
	pc_check(f_ffsll(1LL << 40) == 41, "ffsll(2^40) was not 41");
	pc_check(f_ffsll((1LL << 40) | (1LL << 50)) == 41, "ffsll(2^40 + 2^50) was not 41");
	pc_check(f_ffsll(1LL << 62) == 63, "ffsll(2^62) was not 63");
	if (vbits_long() > 40) {
		volatile int sh = 40;
		pc_check(f_ffsl(1L << sh) == 41, "ffsl(2^40) was not 41");
	}
	if (vbits_long() > 50) {
		volatile int sh = 50, lo = 40;
		pc_check(f_ffsl((1L << lo) | (1L << sh)) == 41, "ffsl(2^40 + 2^50) was not 41");
	}
	pc_pass();
}
