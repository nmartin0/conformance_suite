/*
 * mrand48() and jrand48() when X is 2^47 or more.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.drand48.004
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
	u64 x;
	int i, high_half = 0;
	unsigned short w[3] = { 0x0123, 0x4567, 0x0089 };

	pc_begin("XSH.drand48.004");
	pc_test();
	srand48(31337);
	x = seedx(31337);
	for (i = 0; i < 200; i++) {
		long v = mrand48();

		x = nextx(x);
		if (x >= (1ULL << 47)) {
			long long e = (long long)(x >> 16) - 4294967296LL;

			high_half++;
			pc_check((long long)v == e, "mrand48() value %d is %ld, expected %lld", i, v, e);
		}
	}
	x = pack(w);
	for (i = 0; i < 200; i++) {
		long v = jrand48(w);

		x = nextx(x);
		if (x >= (1ULL << 47)) {
			long long e = (long long)(x >> 16) - 4294967296LL;

			high_half++;
			pc_check((long long)v == e, "jrand48() value %d is %ld, expected %lld", i, v, e);
		}
	}
	pc_check(high_half > 0, "no value with X of 2^47 or more was seen");
	pc_pass();
}
