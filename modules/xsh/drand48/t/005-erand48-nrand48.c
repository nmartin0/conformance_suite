/*
 * erand48() and nrand48() use the caller's array.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.drand48.005
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

typedef unsigned long long u64;

#define MASK 0xFFFFFFFFFFFFULL

/* The generator the page defines: X' = (a X + c) mod 2^48. */
static u64 A = 0x5DEECE66DULL, C = 0xBULL;

static u64 nextx(u64 x)
{
	return (A * x + C) & MASK;
}

/* X after srand48(s): high 32 bits from the low 32 bits of s, low 16 bits 0x330E. */
static u64 seedx(long s)
{
	return (((u64)(unsigned long)s & 0xFFFFFFFFULL) << 16) | 0x330E;
}

/* A 3-word array holds X with word 0 least significant. */
static u64 pack(const unsigned short *w)
{
	return (u64)w[0] | (u64)w[1] << 16 | (u64)w[2] << 32;
}

static void unpack(u64 x, unsigned short *w)
{
	w[0] = (unsigned short)(x & 0xFFFF);
	w[1] = (unsigned short)((x >> 16) & 0xFFFF);
	w[2] = (unsigned short)((x >> 32) & 0xFFFF);
}

static double scaled(u64 x)
{
	return (double)x / 281474976710656.0;	/* 2^48, exact */
}

int main(void)
{
	unsigned short w[3] = { 0x1234, 0xABCD, 0x0F0F }, v[3] = { 0xBEEF, 0x0042, 0x7001 };
	u64 x = pack(w), y = pack(v);
	int i;

	pc_begin("XSH.drand48.005");
	pc_test();
	for (i = 0; i < 40; i++) {
		double d;

		x = nextx(x);
		d = erand48(w);
		pc_check(d >= 0.0 && d < 1.0, "erand48() returned %.17g, outside [0, 1)", d);
		pc_check(d == scaled(x), "erand48() value %d is %.17g, expected %.17g", i, d, scaled(x));
		pc_check(pack(w) == x, "erand48() did not leave the new X in the array");
	}
	for (i = 0; i < 40; i++) {
		long n;

		y = nextx(y);
		n = nrand48(v);
		pc_check(n >= 0 && n <= 2147483647L, "nrand48() returned %ld, outside [0, 2^31)", n);
		pc_check(n == (long)(y >> 17), "nrand48() value %d is %ld, expected %ld", i, n, (long)(y >> 17));
		pc_check(pack(v) == y, "nrand48() did not leave the new X in the array");
	}
	pc_pass();
}
