/*
 * random() range and srandom() repeatability.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.random.001
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static void run(unsigned seed, long *out)
{
	int i;

	srandom(seed);
	for (i = 0; i < 100; i++)
		out[i] = random();
}

int main(void)
{
	static const unsigned seeds[] = { 1, 2, 12345, 0, 0xFFFFFFFFu };
	long a[100], b[100];
	size_t s;
	int i, differ = 0;
	long first;

	pc_begin("XSH.random.001");
	pc_test();
	srandom(9);
	first = random();
	for (i = 0; i < 1000; i++) {
		long v = i == 0 ? first : random();

		pc_check(v >= 0 && v <= 2147483647L, "random() returned %ld, outside [0, 2^31-1]", v);
		if (v != first)
			differ = 1;
	}
	pc_check(differ, "1000 consecutive values were all equal");
	for (s = 0; s < sizeof seeds / sizeof seeds[0]; s++) {
		run(seeds[s], a);
		run(seeds[s], b);
		pc_check(memcmp(a, b, sizeof a) == 0, "seed %u: the sequence was not repeated", seeds[s]);
	}
	pc_pass();
}
