/*
 * bsearch() on an empty array.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.bsearch.003
 */
#include "pc.h"
#include <stdlib.h>

static int calls;

static int cmp(const void *k, const void *e)
{
	(void)k;
	(void)e;
	calls++;
	return 0;
}

int main(void)
{
	int arr[4] = { 1, 2, 3, 4 }, key = 2;
	void *r;

	pc_begin("XSH.bsearch.003");
	pc_test();
	r = bsearch(&key, arr, 0, sizeof arr[0], cmp);
	pc_check(r == NULL, "bsearch() with nel 0 returned a pointer");
	pc_check(calls == 0, "the comparison function was called %d times", calls);
	pc_pass();
}
