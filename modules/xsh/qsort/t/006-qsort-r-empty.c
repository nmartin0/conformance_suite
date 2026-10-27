/*
 * qsort_r() with nel zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.qsort.006
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static int calls;

static int cmp(const void *a, const void *b, void *arg)
{
	(void)a;
	(void)b;
	(void)arg;
	calls++;
	return 0;
}

int main(void)
{
	int arr[3] = { 3, 2, 1 }, copy[3], ctx = 0;

	pc_begin("XSH.qsort.006");
	memcpy(copy, arr, sizeof arr);
	pc_test();
	qsort_r(arr, 0, sizeof arr[0], cmp, &ctx);
	pc_check(calls == 0, "the comparison function was called %d times", calls);
	pc_check(memcmp(arr, copy, sizeof arr) == 0, "the array was rearranged");
	pc_pass();
}
