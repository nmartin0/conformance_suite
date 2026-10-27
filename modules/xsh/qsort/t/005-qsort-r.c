/*
 * qsort_r() passes its context to the comparison function.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.qsort.005
 */
#include "pc.h"
#include <stdlib.h>

struct ctx {
	int descending;
	int calls;
	int wrong;
};

static int cmp(const void *a, const void *b, void *arg)
{
	struct ctx *c = arg;
	int x = *(const int *)a, y = *(const int *)b;
	int r = x < y ? -1 : x > y;

	c->calls++;
	return c->descending ? -r : r;
}

static struct ctx *expect;

static int cmp_checked(const void *a, const void *b, void *arg)
{
	if (arg != expect)
		expect->wrong++;
	return cmp(a, b, arg);
}

int main(void)
{
	int a[200], i, pass;
	struct ctx up = { 0, 0, 0 }, down = { 1, 0, 0 };

	pc_begin("XSH.qsort.005");
	pc_test();
	for (pass = 0; pass < 2; pass++) {
		struct ctx *c = pass ? &down : &up;

		for (i = 0; i < 200; i++)
			a[i] = (i * 91) % 199;
		expect = c;
		qsort_r(a, 200, sizeof a[0], cmp_checked, c);
		pc_check(c->calls > 0, "the comparison function was never called");
		pc_check(c->wrong == 0, "%d calls received a different context pointer", c->wrong);
		for (i = 1; i < 200; i++)
			pc_check(pass ? a[i - 1] >= a[i] : a[i - 1] <= a[i], "pass %d: out of order at %d", pass, i);
	}
	pc_pass();
}
