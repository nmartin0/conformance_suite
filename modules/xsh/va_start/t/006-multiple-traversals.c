/*
 * Several va_start()/va_end() traversals.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.va_start.006
 */
#include "pc.h"
#include <stdarg.h>

static int sums[3];

static void reader(int n, ...)
{
	int pass, i;

	for (pass = 0; pass < 3; pass++) {
		va_list ap;
		int s = 0;

		va_start(ap, n);
		for (i = 0; i < n; i++)
			s = s * 3 + va_arg(ap, int);
		va_end(ap);
		sums[pass] = s;
	}
}

int main(void)
{
	pc_begin("XSH.va_start.006");
	pc_test();
	reader(5, 1, 2, 3, 4, 5);
	pc_check(sums[0] == ((((1 * 3 + 2) * 3 + 3) * 3 + 4) * 3 + 5), "first traversal gave %d", sums[0]);
	pc_check(sums[1] == sums[0], "second traversal gave %d, first gave %d", sums[1], sums[0]);
	pc_check(sums[2] == sums[0], "third traversal gave %d, first gave %d", sums[2], sums[0]);
	pc_pass();
}
