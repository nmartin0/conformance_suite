/*
 * malloc(0) gives a null pointer or a distinct freeable pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.malloc.004
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	void *a[8];
	size_t i, j;

	pc_begin("XSH.malloc.004");
	pc_test();
	for (i = 0; i < 8; i++)
		a[i] = malloc(0);
	for (i = 0; i < 8; i++)
		for (j = i + 1; j < 8; j++)
			pc_check(a[i] == NULL || a[j] == NULL || a[i] != a[j],
			    "two live malloc(0) results are the same pointer");
	for (i = 0; i < 8; i++)
		free(a[i]);
	/* The allocator still works afterwards. */
	{
		char *q = malloc(10);

		pc_check(q != NULL, "malloc(10) failed after zero-size requests");
		q[9] = 'x';
		free(q);
	}
	pc_pass();
}
