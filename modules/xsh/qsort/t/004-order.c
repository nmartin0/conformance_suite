/*
 * qsort() follows the comparison function.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.qsort.004
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static int desc(const void *a, const void *b)
{
	int x = *(const int *)a, y = *(const int *)b;

	return x < y ? 1 : x > y ? -1 : 0;
}

static int bystr(const void *a, const void *b)
{
	return strcmp(*(const char *const *)a, *(const char *const *)b);
}

int main(void)
{
	int a[100], i;
	const char *s[] = { "pear", "apple", "fig", "banana", "cherry", "apricot", "date", "elderberry", "grape", "kiwi", "" , "Zed" };
	size_t n = sizeof s / sizeof s[0], k;

	pc_begin("XSH.qsort.004");
	for (i = 0; i < 100; i++)
		a[i] = (i * 37) % 101;
	pc_test();
	qsort(a, 100, sizeof a[0], desc);
	for (i = 1; i < 100; i++)
		pc_check(a[i - 1] >= a[i], "not descending at %d", i);
	qsort(s, n, sizeof s[0], bystr);
	for (k = 1; k < n; k++)
		pc_check(strcmp(s[k - 1], s[k]) <= 0, "strings out of order at %lu: %s, %s", (unsigned long)k, s[k - 1], s[k]);
	pc_pass();
}
