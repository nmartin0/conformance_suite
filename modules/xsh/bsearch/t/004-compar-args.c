/*
 * bsearch() calls the comparison function with the key and an element.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.bsearch.004
 */
#include "pc.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define W 7
#define N 50

static unsigned char rec[N][W];
static const void *keyaddr;
static int calls, bad_first, bad_second;

/* Records hold a 2-byte big-endian number first, then 5 payload bytes. */
static int cmp(const void *k, const void *e)
{
	const unsigned char *a = k, *b = e;
	uintptr_t off = (uintptr_t)b - (uintptr_t)rec;
	int x = a[0] * 256 + a[1], y = b[0] * 256 + b[1];

	calls++;
	if (k != keyaddr)
		bad_first++;
	if ((uintptr_t)b < (uintptr_t)rec || off >= sizeof rec || off % W != 0)
		bad_second++;
	return x < y ? -1 : x > y;
}

int main(void)
{
	unsigned char key[W];
	int i, j;

	pc_begin("XSH.bsearch.004");
	for (i = 0; i < N; i++) {
		rec[i][0] = (unsigned char)((i * 13) >> 8);
		rec[i][1] = (unsigned char)((i * 13) & 255);
		for (j = 2; j < W; j++)
			rec[i][j] = (unsigned char)(i + j);
	}
	keyaddr = key;
	pc_test();
	for (i = 0; i < N; i++) {
		void *r;

		memcpy(key, rec[i], W);
		calls = 0;
		r = bsearch(key, rec, N, W, cmp);
		pc_check(calls > 0, "no comparison for record %d", i);
		pc_check(r == rec[i], "record %d: got %p, expected %p", i, r, (void *)rec[i]);
	}
	pc_check(bad_first == 0, "%d calls had a first argument that was not the key", bad_first);
	pc_check(bad_second == 0, "%d calls had a second argument that was not an element", bad_second);
	pc_pass();
}
