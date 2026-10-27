/*
 * srand48() seeds the generator as the page defines.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.drand48.001
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
	static const long seeds[] = { 0, 1, 12345, 0x7FFFFFFFL, -1, -7 };
	size_t s;
	int i;

	pc_begin("XSH.drand48.001");
	pc_test();
	for (s = 0; s < sizeof seeds / sizeof seeds[0]; s++) {
		u64 x = seedx(seeds[s]);

		srand48(seeds[s]);
		for (i = 0; i < 30; i++) {
			long v;

			x = nextx(x);
			v = lrand48();
			pc_check(v == (long)(x >> 17), "seed %ld: value %d is %ld, expected %ld", seeds[s], i, v, (long)(x >> 17));
		}
	}
	if (sizeof(long) > 4) {
		long hi = (long)(0x7ABC00000000LL | 5), lo = 5;
		long a[10], b[10];

		srand48(hi);
		for (i = 0; i < 10; i++)
			a[i] = lrand48();
		srand48(lo);
		for (i = 0; i < 10; i++)
			b[i] = lrand48();
		pc_check(memcmp(a, b, sizeof a) == 0, "bits above the low 32 of the seed changed the sequence");
	}
	pc_pass();
}
