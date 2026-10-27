/*
 * lfind() finds an entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lfind.001
 */
#include "pc.h"
#include <search.h>
#include <stdlib.h>

static int eq(const void *a, const void *b)
{
	return *(const int *)a != *(const int *)b;
}

int main(void)
{
	int tab[30], i;
	size_t n = 30;

	pc_begin("XSH.lfind.001");
	for (i = 0; i < 30; i++)
		tab[i] = i * i + 1;
	pc_test();
	for (i = 0; i < 30; i++) {
		int key = tab[i];
		int *r = lfind(&key, tab, &n, sizeof tab[0], eq);

		pc_check(r == &tab[i], "entry %d: got %p, expected %p", i, (void *)r, (void *)&tab[i]);
		pc_check(n == 30, "the count changed to %lu", (unsigned long)n);
	}
	pc_pass();
}
