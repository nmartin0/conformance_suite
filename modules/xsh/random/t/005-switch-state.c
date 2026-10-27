/*
 * setstate() switches generators and keeps each one's place.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.random.005
 */
#include "pc.h"
#include <stdlib.h>

static long sa[256 / sizeof(long) + 1], sb[256 / sizeof(long) + 1];

int main(void)
{
	long ref_a[10], ref_b[5], got_a[10], got_b[5];
	int i;

	pc_begin("XSH.random.005");
	pc_test();
	/* Reference runs, uninterrupted. */
	pc_check(initstate(11, (char *)sa, 128) != NULL, "initstate A failed");
	for (i = 0; i < 10; i++)
		ref_a[i] = random();
	pc_check(initstate(22, (char *)sb, 128) != NULL, "initstate B failed");
	for (i = 0; i < 5; i++)
		ref_b[i] = random();
	/* The same, interleaved: A, B, A. */
	pc_check(initstate(11, (char *)sa, 128) != NULL, "initstate A failed");
	for (i = 0; i < 5; i++)
		got_a[i] = random();
	pc_check(initstate(22, (char *)sb, 128) != NULL, "initstate B failed");
	for (i = 0; i < 5; i++)
		got_b[i] = random();
	pc_check(setstate((char *)sa) == (char *)sb, "setstate(A) did not return B, the array in use");
	for (i = 5; i < 10; i++)
		got_a[i] = random();
	for (i = 0; i < 10; i++)
		pc_check(got_a[i] == ref_a[i], "A's value %d differs after switching away and back", i);
	for (i = 0; i < 5; i++)
		pc_check(got_b[i] == ref_b[i], "B's value %d differs from a fresh run", i);
	pc_pass();
}
