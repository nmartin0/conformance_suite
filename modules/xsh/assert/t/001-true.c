/*
 * assert() of a true expression continues.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.assert.001
 */
#include "pc.h"
#include <assert.h>
#include <stdlib.h>

int main(void)
{
	int n = 0, x = 5;
	int *p = &x;
	double d = 2.5;
	long l = 7L;

	pc_begin("XSH.assert.001");
	pc_test();
	assert(1);
	assert(x == 5);
	assert(p);
	assert(d);
	assert(l);
	assert(++n);
	pc_check(n >= 1, "the expression of assert() was not evaluated");
	assert((n = n + 10) != 0);
	pc_check(n >= 11, "the expression of assert() was not evaluated");
	pc_pass();
}
