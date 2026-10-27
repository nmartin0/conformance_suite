/*
 * lsearch() of a present entry adds nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lfind.003
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
	int tab[20], copy[20], i;
	size_t n = 12;

	pc_begin("XSH.lfind.003");
	for (i = 0; i < 20; i++)
		tab[i] = 100 + i * 5;
	memcpy(copy, tab, sizeof tab);
	pc_test();
	for (i = 0; i < 12; i++) {
		int key = tab[i];
		int *r = lsearch(&key, tab, &n, sizeof tab[0], eq);

		pc_check(r == &tab[i], "entry %d: got %p, expected %p", i, (void *)r, (void *)&tab[i]);
		pc_check(n == 12, "the count changed to %lu", (unsigned long)n);
	}
	pc_check(memcmp(tab, copy, sizeof tab) == 0, "the table was altered");
	pc_pass();
}
