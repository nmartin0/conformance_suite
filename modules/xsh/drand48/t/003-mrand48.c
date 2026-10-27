/*
 * mrand48() and jrand48() range and value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.drand48.003
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
	long lo = -2147483647L - 1, hi = 2147483647L;
	u64 x;
	int i, low_half = 0;
	unsigned short w[3] = { 0x0123, 0x4567, 0x0089 };

	pc_begin("XSH.drand48.003");
	pc_test();
	srand48(31337);
	x = seedx(31337);
	for (i = 0; i < 200; i++) {
		long v = mrand48();

		x = nextx(x);
		pc_check(v >= lo && v <= hi, "mrand48() returned %ld, outside [-2^31, 2^31)", v);
		if (x < (1ULL << 47)) {
			low_half++;
			pc_check(v == (long)(x >> 16), "mrand48() value %d is %ld, expected %ld", i, v, (long)(x >> 16));
		}
	}
	x = pack(w);
	for (i = 0; i < 200; i++) {
		long v = jrand48(w);

		x = nextx(x);
		pc_check(v >= lo && v <= hi, "jrand48() returned %ld, outside [-2^31, 2^31)", v);
		if (x < (1ULL << 47))
			pc_check(v == (long)(x >> 16), "jrand48() value %d is %ld, expected %ld", i, v, (long)(x >> 16));
		pc_check(pack(w) == x, "jrand48() did not leave the new X in the array");
	}
	pc_check(low_half > 0, "no value with X below 2^47 was seen; the exact check ran on nothing");
	pc_pass();
}
