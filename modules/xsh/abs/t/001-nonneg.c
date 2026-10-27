/*
 * abs() returns a non-negative argument unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.abs.001
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.abs.001");
	pc_test();
	static const int in[] = { 0, 1, 5, 12345, INT_MAX };
	volatile int i, r;
	int k;

	for (k = 0; k < 5; k++) {
		i = in[k];
		r = abs(i);
		pc_check(r == in[k], "abs(%d) returned %d", in[k], (int)r);
	}
	pc_pass();
}
