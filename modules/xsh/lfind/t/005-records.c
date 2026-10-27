/*
 * lsearch() and lfind() on string records.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lfind.005
 */
#include "pc.h"
#include <search.h>
#include <stdlib.h>
#include <string.h>

#define W 8

static int cmp(const void *a, const void *b)
{
	return strcmp(a, b) != 0;
}

int main(void)
{
	static const char *const words[] = { "alpha", "beta", "gamma", "delta", "eps", "zeta" };
	char tab[10][W];
	size_t n = 0, i;

	pc_begin("XSH.lfind.005");
	memset(tab, 0, sizeof tab);
	pc_test();
	for (i = 0; i < 6; i++) {
		char key[W];
		void *r;

		memset(key, 0, W);
		strcpy(key, words[i]);
		pc_check(lfind(key, tab, &n, W, cmp) == NULL, "%s was found before it was added", words[i]);
		r = lsearch(key, tab, &n, W, cmp);
		pc_check(r == tab[i], "%s: lsearch() returned %p, expected record %lu", words[i], r, (unsigned long)i);
		pc_check(n == i + 1, "the count is %lu after %lu additions", (unsigned long)n, (unsigned long)i + 1);
		pc_check(strcmp(tab[i], words[i]) == 0, "record %lu holds %s", (unsigned long)i, tab[i]);
	}
	for (i = 0; i < 6; i++) {
		char other[W];

		memset(other, 0, W);
		strcpy(other, words[i]);
		pc_check(lfind(other, tab, &n, W, cmp) == tab[i], "%s was not found by content", words[i]);
	}
	pc_pass();
}
