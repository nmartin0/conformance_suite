/*
 * Separate streams do not affect each other.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.drand48.006
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
	unsigned short a[3] = { 1, 2, 3 }, b[3] = { 9, 8, 7 }, a2[3] = { 1, 2, 3 };
	double ref[20], got[20];
	int i;

	pc_begin("XSH.drand48.006");
	pc_test();
	for (i = 0; i < 20; i++)
		ref[i] = erand48(a);
	srand48(5);
	for (i = 0; i < 20; i++) {
		(void)drand48();
		(void)lrand48();
		(void)nrand48(b);
		got[i] = erand48(a2);
		(void)jrand48(b);
		(void)mrand48();
	}
	for (i = 0; i < 20; i++)
		pc_check(got[i] == ref[i], "stream value %d changed because other generators were used", i);
	pc_check(memcmp(a, a2, sizeof a) == 0, "the stream's final state differs");
	pc_pass();
}
