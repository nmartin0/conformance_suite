/*
 * lsearch() appends an absent entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lfind.004
 */
#include "pc.h"
#include <search.h>
#include <stdlib.h>
#include <string.h>

static int eq(const void *a, const void *b)
{
	return *(const int *)a != *(const int *)b;
}

int main(void)
{
	int tab[40], i;
	size_t n = 0;

	pc_begin("XSH.lfind.004");
	for (i = 0; i < 40; i++)
		tab[i] = -1;
	pc_test();
	for (i = 0; i < 25; i++) {
		int key = 1000 + i * 3, before[40];
		int *r;

		memcpy(before, tab, sizeof tab);
		r = lsearch(&key, tab, &n, sizeof tab[0], eq);
		pc_check(n == (size_t)i + 1, "after %d additions the count is %lu", i + 1, (unsigned long)n);
		pc_check(r == &tab[i], "addition %d: returned %p, expected the position after the last entry %p",
		    i, (void *)r, (void *)&tab[i]);
		pc_check(tab[i] == key, "the new entry holds %d, expected %d", tab[i], key);
		pc_check(memcmp(before, tab, i * sizeof tab[0]) == 0, "earlier entries changed");
		pc_check(lfind(&key, tab, &n, sizeof tab[0], eq) == &tab[i], "lfind() did not find the new entry");
		r = lsearch(&key, tab, &n, sizeof tab[0], eq);
		pc_check(r == &tab[i] && n == (size_t)i + 1, "a second lsearch() added or moved the entry");
	}
	pc_pass();
}
