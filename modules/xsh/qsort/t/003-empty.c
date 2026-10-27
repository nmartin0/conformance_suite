/*
 * qsort() with nel zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.qsort.003
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static int calls;

static int cmp(const void *a, const void *b)
{
	(void)a;
	(void)b;
	calls++;
	return 0;
}

int main(void)
{
	int arr[5] = { 5, 4, 3, 2, 1 };
	int copy[5];

	pc_begin("XSH.qsort.003");
	memcpy(copy, arr, sizeof arr);
	pc_test();
	qsort(arr, 0, sizeof arr[0], cmp);
	pc_check(calls == 0, "the comparison function was called %d times", calls);
	pc_check(memcmp(arr, copy, sizeof arr) == 0, "the array was rearranged");
	pc_pass();
}
