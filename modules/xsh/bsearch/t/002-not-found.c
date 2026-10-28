/*
 * bsearch() returns a null pointer when nothing matches.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.bsearch.002
 */
#include "pc.h"
#include <stdlib.h>

static const void *want;
static int ncmp, badkey;

static int cmp(const void *k, const void *e)
{
	int a = *(const int *)k, b = *(const int *)e;

	ncmp++;
	if (k != want)
		badkey++;
	return a < b ? -1 : a > b;
}

/* Searches for *kp; a no-match answer needs the comparison function, always given the key first. */
static void *search(int *kp, int *a, size_t n, const char *what)
{
	void *r;

	want = kp;
	ncmp = badkey = 0;
	r = bsearch(kp, a, n, sizeof a[0], cmp);
	pc_check(ncmp > 0, "%s: the comparison function was never called", what);
	pc_check(badkey == 0, "%s: the comparison function's first argument was not the key", what);
	return r;
}

int main(void)
{
	int arr[20], i, key;

	pc_begin("XSH.bsearch.002");
	for (i = 0; i < 20; i++)
		arr[i] = i * 10;
	pc_test();
	key = -1;
	pc_check(search(&key, arr, 20, "below the first") == NULL, "key below the first element was found");
	key = 191;
	pc_check(search(&key, arr, 20, "above the last") == NULL, "key above the last element was found");
	for (i = 0; i < 19; i++) {
		key = i * 10 + 5;
		pc_check(search(&key, arr, 20, "between elements") == NULL, "key %d between elements was found", key);
	}
	key = 6;
	pc_check(search(&key, arr, 1, "one element") == NULL, "non-matching key found in a 1-element array");
	key = -6;
	pc_check(search(&key, arr, 1, "one element") == NULL, "non-matching key found in a 1-element array");
	pc_pass();
}
