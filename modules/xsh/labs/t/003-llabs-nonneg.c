/*
 * llabs() returns a non-negative argument unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.labs.003
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.labs.003");
	pc_test();
	static const long long in[] = { 0LL, 1LL, 5000000000LL, LLONG_MAX };
	volatile long long i, r;
	int k;

	for (k = 0; k < 4; k++) {
		i = in[k];
		r = llabs(i);
		pc_check(r == in[k], "llabs(%lld) returned %lld", in[k], (long long)r);
	}
	pc_pass();
}
