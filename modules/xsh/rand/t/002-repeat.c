/*
 * srand() with the same seed repeats the sequence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.rand.002
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static void run(unsigned seed, int *out)
{
	int i;

	srand(seed);
	for (i = 0; i < 100; i++)
		out[i] = rand();
}

int main(void)
{
	static const unsigned seeds[] = { 1, 42, 0, UINT_MAX };
	int a[100], b[100], c[100];
	size_t s;

	pc_begin("XSH.rand.002");
	pc_test();
	for (s = 0; s < sizeof seeds / sizeof seeds[0]; s++) {
		run(seeds[s], a);
		run(seeds[s], b);
		pc_check(memcmp(a, b, sizeof a) == 0, "seed %u: the sequence was not repeated", seeds[s]);
	}
	run(7, a);
	run(8, c);
	run(7, b);
	pc_check(memcmp(a, b, sizeof a) == 0, "seed 7 after another seed did not repeat its sequence");
	pc_pass();
}
