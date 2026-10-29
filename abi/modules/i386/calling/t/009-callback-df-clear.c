/*
 * A comparison function called by qsort or bsearch is entered with the direction flag clear.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.009
 */
#include "../../../../check.h"
#include <stdlib.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.009")
#else
ABI_TARGET int cmp_body(const void *a, const void *b)
{
	int x = *(const int *)a, y = *(const int *)b;

	return (x > y) - (x < y);
}

int cmp_tramp(const void *a, const void *b);
ABI_ENTRY_TRAMPOLINE(cmp_tramp, cmp_body);

/* Make the library call the comparison function a good many times, with sizes that vary. */
static void run_callbacks(void)
{
	static int a[64];
	int i, key, *hit;

	for (i = 0; i < 64; i++)
		a[i] = (i * 37) % 64;
	qsort(a, 64, sizeof a[0], cmp_tramp);
	for (i = 0; i < 64; i++)
		pc_check(a[i] == i, "qsort left a[%d] = %d", i, a[i]);
	for (key = 0; key < 64; key += 9) {
		hit = bsearch(&key, a, 64, sizeof a[0], cmp_tramp);
		pc_check(hit == &a[key], "bsearch did not find %d", key);
	}
}
int main(void)
{
	pc_begin("ABI.i386.calling.009");
	pc_test();
	run_callbacks();
	abi_expect_entry_df_clear(&cmp_tramp_rec, "the qsort/bsearch comparison function");
	pc_pass();
}
#endif
