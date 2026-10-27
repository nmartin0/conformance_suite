/*
 * div() satisfies quot*denom+rem == numer for extreme values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.div.003
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.div.003");
	pc_test();
	volatile int n, d;
	div_t res;
	int nums[6], dens[6];
	int a, b;

	nums[0] = 0; nums[1] = 1; nums[2] = -1; nums[3] = INT_MAX; nums[4] = -INT_MAX; nums[5] = 12345;
	dens[0] = 1; dens[1] = -1; dens[2] = 2; dens[3] = -2; dens[4] = INT_MAX; dens[5] = -7;
	for (a = 0; a < 6; a++) {
		for (b = 0; b < 6; b++) {
			n = nums[a];
			d = dens[b];
			res = div(n, d);
			pc_check(res.quot * dens[b] + res.rem == nums[a],
			    "div(%lld, %lld) gave quot %lld rem %lld, which do not satisfy quot*denom+rem == numer",
			    (long long)nums[a], (long long)dens[b], (long long)res.quot, (long long)res.rem);
			pc_check(res.rem == 0 || ((res.rem < 0) == (nums[a] < 0)),
			    "div(%lld, %lld) gave remainder %lld, whose sign differs from the numerator's",
			    (long long)nums[a], (long long)dens[b], (long long)res.rem);
		}
	}
	pc_pass();
}
