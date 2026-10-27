/*
 * qsort() sorts ints ascending.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.qsort.001
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static int cmp(const void *a, const void *b)
{
	int x = *(const int *)a, y = *(const int *)b;

	return x < y ? -1 : x > y;
}

static unsigned long seed = 12345;

static unsigned rnd(void)
{
	seed = seed * 6364136223846793005UL + 1442695040888963407UL;
	return (unsigned)(seed >> 33);
}

#define MAXN 50000

int main(void)
{
	static const int sizes[] = { 1, 2, 3, 10, 1000, 50000 };
	static int a[MAXN], count[1024];
	size_t s;
	int pat, n, i;

	pc_begin("XSH.qsort.001");
	pc_test();
	for (s = 0; s < sizeof sizes / sizeof sizes[0]; s++) {
		n = sizes[s];
		for (pat = 0; pat < 5; pat++) {
			memset(count, 0, sizeof count);
			for (i = 0; i < n; i++) {
				switch (pat) {
				case 0: a[i] = (int)(rnd() % 1000); break;
				case 1: a[i] = i; break;
				case 2: a[i] = n - i; break;
				case 3: a[i] = 7; break;
				default: a[i] = (int)(rnd() % 3); break;
				}
				if (n <= 1000)
					count[a[i] % 1024]++;
			}
			qsort(a, (size_t)n, sizeof a[0], cmp);
			for (i = 1; i < n; i++)
				pc_check(a[i - 1] <= a[i], "n=%d pattern %d: a[%d]=%d > a[%d]=%d", n, pat, i - 1, a[i - 1], i, a[i]);
			if (n <= 1000) {
				for (i = 0; i < n; i++)
					count[a[i] % 1024]--;
				for (i = 0; i < 1024; i++)
					pc_check(count[i] == 0, "n=%d pattern %d: the values are not a permutation of the original", n, pat);
			}
		}
	}
	pc_pass();
}
