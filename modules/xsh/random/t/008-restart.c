/*
 * srandom() restarts a generator set up with initstate().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.random.008
 */
#include "pc.h"
#include <stdlib.h>

static long st[256 / sizeof(long) + 1];

int main(void)
{
	static const size_t sizes[] = { 8, 32, 128 };
	size_t k;

	pc_begin("XSH.random.008");
	pc_test();
	for (k = 0; k < 3; k++) {
		long a[30], b[30], c[30], d[30];
		int i;

		pc_check(initstate(77, (char *)st, sizes[k]) != NULL, "initstate(size %lu) failed", (unsigned long)sizes[k]);
		for (i = 0; i < 30; i++)
			a[i] = random();
		pc_check(initstate(77, (char *)st, sizes[k]) != NULL, "second initstate(size %lu) failed", (unsigned long)sizes[k]);
		for (i = 0; i < 30; i++)
			b[i] = random();
		for (i = 0; i < 30; i++)
			pc_check(a[i] == b[i], "size %lu: value %d differs after initstate() with the same seed", (unsigned long)sizes[k], i);
		/* srandom() on this array with one seed twice gives one sequence. */
		srandom(78);
		for (i = 0; i < 30; i++)
			c[i] = random();
		srandom(78);
		for (i = 0; i < 30; i++)
			d[i] = random();
		for (i = 0; i < 30; i++)
			pc_check(c[i] == d[i], "size %lu: value %d differs after srandom() with the same seed", (unsigned long)sizes[k], i);
	}
	pc_pass();
}
