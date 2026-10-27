/*
 * calloc() with a zero count or size.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.calloc.003
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	void *a[8];
	size_t i, j;

	pc_begin("XSH.calloc.003");
	pc_test();
	for (i = 0; i < 8; i++)
		a[i] = (i & 1) ? calloc(0, 16) : calloc(16, 0);
	for (i = 0; i < 8; i++)
		for (j = i + 1; j < 8; j++)
			pc_check(a[i] == NULL || a[j] == NULL || a[i] != a[j],
			    "two live zero-size calloc() results are the same pointer");
	for (i = 0; i < 8; i++)
		free(a[i]);
	{
		char *q = calloc(4, 4);

		pc_check(q != NULL, "calloc(4, 4) failed after zero-size requests");
		free(q);
	}
	pc_pass();
}
