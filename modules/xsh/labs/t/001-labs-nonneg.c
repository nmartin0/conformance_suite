/*
 * labs() returns a non-negative argument unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.labs.001
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.labs.001");
	pc_test();
	static const long in[] = { 0L, 1L, 5L, 123456L, LONG_MAX };
	volatile long i, r;
	int k;

	for (k = 0; k < 5; k++) {
		i = in[k];
		r = labs(i);
		pc_check(r == in[k], "labs(%ld) returned %ld", in[k], (long)r);
	}
	pc_pass();
}
