/*
 * qsort() moves records whole.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.qsort.002
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

#define N 500
#define W 12

static unsigned char rec[N][W];
static int seen[N];

static unsigned key_of(const unsigned char *r)
{
	return (unsigned)r[0] | (unsigned)r[1] << 8 | (unsigned)r[2] << 16 | (unsigned)r[3] << 24;
}

static int cmp(const void *a, const void *b)
{
	unsigned x = key_of(a), y = key_of(b);

	return x < y ? -1 : x > y;
}

int main(void)
{
	unsigned long seed = 99;
	int i, j;

	pc_begin("XSH.qsort.002");
	for (i = 0; i < N; i++) {
		unsigned k;

		seed = seed * 6364136223846793005UL + 1442695040888963407UL;
		k = (unsigned)(seed >> 33) % 100000 + 1;
		/* Distinct keys: use the index in the low digits. */
		k = k * 1000 + (unsigned)i;
		rec[i][0] = (unsigned char)k;
		rec[i][1] = (unsigned char)(k >> 8);
		rec[i][2] = (unsigned char)(k >> 16);
		rec[i][3] = (unsigned char)(k >> 24);
		for (j = 0; j < 8; j++)
			rec[i][4 + j] = (unsigned char)(k * 31 + (unsigned)j * 7);
	}
	pc_test();
	qsort(rec, N, W, cmp);
	for (i = 0; i < N; i++) {
		unsigned k = key_of(rec[i]);
		int orig = (int)(k % 1000);

		if (i > 0)
			pc_check(key_of(rec[i - 1]) < k, "keys not ascending at %d", i);
		for (j = 0; j < 8; j++)
			pc_check(rec[i][4 + j] == (unsigned char)(k * 31 + (unsigned)j * 7), "record %d was damaged", i);
		pc_check(orig >= 0 && orig < N && !seen[orig], "record with original index %d is missing or repeated", orig);
		seen[orig] = 1;
	}
	pc_pass();
}
