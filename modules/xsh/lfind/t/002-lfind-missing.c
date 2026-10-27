/*
 * lfind() of an absent entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lfind.002
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
	int tab[11], copy[11], i, key;
	size_t n = 10, zero = 0;

	pc_begin("XSH.lfind.002");
	for (i = 0; i < 11; i++)
		tab[i] = i * 2;
	memcpy(copy, tab, sizeof tab);
	pc_test();
	key = 7;
	pc_check(lfind(&key, tab, &n, sizeof tab[0], eq) == NULL, "absent key 7 was found");
	key = 20;	/* equals the slot just past the table, which is not part of it */
	pc_check(lfind(&key, tab, &n, sizeof tab[0], eq) == NULL, "key beyond the last entry was found");
	pc_check(n == 10, "the count changed to %lu", (unsigned long)n);
	pc_check(memcmp(tab, copy, sizeof tab) == 0, "the table was altered");
	key = 0;
	pc_check(lfind(&key, tab, &zero, sizeof tab[0], eq) == NULL, "an entry was found in an empty table");
	pc_check(zero == 0, "the count of an empty table changed");
	pc_pass();
}
