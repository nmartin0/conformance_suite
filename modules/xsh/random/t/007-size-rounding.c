/*
 * initstate() rounds sizes down.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.random.007
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static long st[512 / sizeof(long) + 1];

static void run(size_t size, long *out)
{
	int i;

	pc_check(initstate(1234, (char *)st, size) != NULL, "initstate(size %lu) returned a null pointer", (unsigned long)size);
	for (i = 0; i < 40; i++) {
		out[i] = random();
		pc_check(out[i] >= 0 && out[i] <= 2147483647L, "size %lu: random() returned %ld", (unsigned long)size, out[i]);
	}
}

int main(void)
{
	static const size_t pairs[][2] = {
		{ 20, 8 }, { 31, 8 }, { 33, 32 }, { 100, 64 }, { 127, 64 }, { 255, 128 }, { 300, 256 },
	};
	long a[40], b[40];
	size_t k;

	pc_begin("XSH.random.007");
	pc_test();
	for (k = 0; k < sizeof pairs / sizeof pairs[0]; k++) {
		run(pairs[k][0], a);
		run(pairs[k][1], b);
		pc_check(memcmp(a, b, sizeof a) == 0, "the sequence for size %lu differs from the one for size %lu",
		    (unsigned long)pairs[k][0], (unsigned long)pairs[k][1]);
	}
	pc_pass();
}
