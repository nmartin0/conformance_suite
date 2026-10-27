/*
 * bsearch() finds each element.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.bsearch.001
 */
#include "pc.h"
#include <stdlib.h>

static int cmp(const void *k, const void *e)
{
	int a = *(const int *)k, b = *(const int *)e;

	return a < b ? -1 : a > b;
}

int main(void)
{
	static const int sizes[] = { 1, 2, 3, 10, 257 };
	static int arr[257];
	size_t s;

	pc_begin("XSH.bsearch.001");
	pc_test();
	for (s = 0; s < sizeof sizes / sizeof sizes[0]; s++) {
		int n = sizes[s], i;

		for (i = 0; i < n; i++)
			arr[i] = i * 3 - 100;
		for (i = 0; i < n; i++) {
			int key = arr[i];
			int *r = bsearch(&key, arr, (size_t)n, sizeof arr[0], cmp);

			pc_check(r == &arr[i], "n=%d: key %d: got %p, expected element %d", n, key, (void *)r, i);
		}
	}
	pc_pass();
}
