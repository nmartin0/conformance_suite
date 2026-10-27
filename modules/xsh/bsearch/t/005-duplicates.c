/*
 * bsearch() among equal members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.bsearch.005
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

static int cmp(const void *k, const void *e)
{
	int a = *(const int *)k, b = *(const int *)e;

	return a < b ? -1 : a > b;
}

int main(void)
{
	int arr[40], copy[40], i, key;
	int *r;

	pc_begin("XSH.bsearch.005");
	for (i = 0; i < 40; i++)
		arr[i] = i < 10 ? 1 : i < 30 ? 5 : 9;
	memcpy(copy, arr, sizeof arr);
	pc_test();
	key = 5;
	r = bsearch(&key, arr, 40, sizeof arr[0], cmp);
	pc_check(r != NULL, "no member returned although 20 members match");
	pc_check(r >= &arr[10] && r < &arr[30], "returned element index %ld is not a matching member", (long)(r - arr));
	key = 1;
	r = bsearch(&key, arr, 40, sizeof arr[0], cmp);
	pc_check(r != NULL && *r == 1, "key 1 not found among duplicates");
	key = 9;
	r = bsearch(&key, arr, 40, sizeof arr[0], cmp);
	pc_check(r != NULL && *r == 9, "key 9 not found among duplicates");
	key = 3;
	pc_check(bsearch(&key, arr, 40, sizeof arr[0], cmp) == NULL, "absent key 3 was found");
	pc_check(memcmp(arr, copy, sizeof arr) == 0, "the array was altered");
	pc_pass();
}
