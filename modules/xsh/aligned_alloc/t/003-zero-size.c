/*
 * aligned_alloc() with size 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.aligned_alloc.003
 */
#include "pc.h"
#include <stddef.h>
#include <stdlib.h>

int main(void)
{
	void *a[6];
	size_t i, j;

	pc_begin("XSH.aligned_alloc.003");
	pc_test();
	for (i = 0; i < 6; i++)
		a[i] = aligned_alloc(_Alignof(max_align_t), 0);
	for (i = 0; i < 6; i++)
		for (j = i + 1; j < 6; j++)
			pc_check(a[i] == NULL || a[j] == NULL || a[i] != a[j],
			    "two live zero-size results are the same pointer");
	for (i = 0; i < 6; i++)
		free(a[i]);
	pc_pass();
}
