/*
 * lcong48() sets X, a and c; srand48() and seed48() restore a and c.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.drand48.009
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
	/*
	 * The words of the multiplier and of X are palindromes, so the result
	 * does not depend on which end of the array holds the low-order word.
	 */
	unsigned short param[7] = { 0x2468, 0x1357, 0x2468, 0x0005, 0xDEED, 0x0005, 0x0123 };
	unsigned short v[3] = { 0x0A0A, 0x0B0B, 0x0C0C };
	u64 x, std_a = 0x5DEECE66DULL, std_c = 0xBULL;
	int i, pass;

	pc_begin("XSH.drand48.009");
	pc_test();
	for (pass = 0; pass < 2; pass++) {
		lcong48(param);
		A = 0x0005DEED0005ULL;
		C = 0x0123;
		x = pack(param);
		for (i = 0; i < 20; i++) {
			long n;

			x = nextx(x);
			n = lrand48();
			pc_check(n == (long)(x >> 17), "pass %d: custom value %d is %ld, expected %ld", pass, i, n, (long)(x >> 17));
		}
		/* A later initialiser puts the standard a and c back. */
		A = std_a;
		C = std_c;
		if (pass == 0) {
			srand48(5);
			x = seedx(5);
		} else {
			(void)seed48(v);
			x = pack(v);
		}
		for (i = 0; i < 20; i++) {
			long n;

			x = nextx(x);
			n = lrand48();
			pc_check(n == (long)(x >> 17), "pass %d: standard value %d is %ld, expected %ld", pass, i, n, (long)(x >> 17));
		}
	}
	pc_pass();
}
