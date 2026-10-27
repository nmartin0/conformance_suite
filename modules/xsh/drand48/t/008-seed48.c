/*
 * seed48() sets X and returns the previous X.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.drand48.008
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
	unsigned short v[3] = { 0x1234, 0x5678, 0x9ABC }, prev[3], *p;
	u64 x;
	int i;

	pc_begin("XSH.drand48.008");
	pc_test();
	srand48(777);
	p = seed48(v);
	pc_check(p != NULL, "seed48() returned a null pointer");
	memcpy(prev, p, sizeof prev);
	x = seedx(777);
	pc_check(pack(prev) == x, "the returned previous X is %#llx, expected %#llx", pack(prev), x);
	x = pack(v);
	for (i = 0; i < 5; i++) {
		long n;

		x = nextx(x);
		n = lrand48();
		pc_check(n == (long)(x >> 17), "after seed48() value %d is %ld, expected %ld", i, n, (long)(x >> 17));
	}
	/* The returned value is the last X generated, so that a run can be resumed. */
	p = seed48(v);
	pc_check(p != NULL, "the second seed48() returned a null pointer");
	pc_check(pack(p) == x, "the returned X is %#llx, expected the last one generated %#llx", pack(p), x);
	pc_pass();
}
