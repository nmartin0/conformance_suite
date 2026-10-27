/*
 * bsearch() returns a null pointer when nothing matches.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.bsearch.002
 */
#include "pc.h"
#include <stdlib.h>

static int cmp(const void *k, const void *e)
{
	int a = *(const int *)k, b = *(const int *)e;

	return a < b ? -1 : a > b;
}

int main(void)
{
	int arr[20], i, key;

	pc_begin("XSH.bsearch.002");
	for (i = 0; i < 20; i++)
		arr[i] = i * 10;
	pc_test();
	key = -1;
	pc_check(bsearch(&key, arr, 20, sizeof arr[0], cmp) == NULL, "key below the first element was found");
	key = 191;
	pc_check(bsearch(&key, arr, 20, sizeof arr[0], cmp) == NULL, "key above the last element was found");
	for (i = 0; i < 19; i++) {
		key = i * 10 + 5;
		pc_check(bsearch(&key, arr, 20, sizeof arr[0], cmp) == NULL, "key %d between elements was found", key);
	}
	key = 6;
	pc_check(bsearch(&key, arr, 1, sizeof arr[0], cmp) == NULL, "non-matching key found in a 1-element array");
	key = -6;
	pc_check(bsearch(&key, arr, 1, sizeof arr[0], cmp) == NULL, "non-matching key found in a 1-element array");
	pc_pass();
}
