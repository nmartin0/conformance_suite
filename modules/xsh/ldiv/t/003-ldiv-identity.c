/*
 * ldiv() satisfies quot*denom+rem == numer for extreme values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldiv.003
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.ldiv.003");
	pc_test();
	volatile long n, d;
	ldiv_t res;
	long nums[6], dens[6];
	int a, b;

	nums[0] = 0; nums[1] = 1; nums[2] = -1; nums[3] = LONG_MAX; nums[4] = -LONG_MAX; nums[5] = 12345;
	dens[0] = 1; dens[1] = -1; dens[2] = 2; dens[3] = -2; dens[4] = LONG_MAX; dens[5] = -7;
	for (a = 0; a < 6; a++) {
		for (b = 0; b < 6; b++) {
			n = nums[a];
			d = dens[b];
			res = ldiv(n, d);
			pc_check(res.quot * dens[b] + res.rem == nums[a],
			    "ldiv(%lld, %lld) gave quot %lld rem %lld, which do not satisfy quot*denom+rem == numer",
			    (long long)nums[a], (long long)dens[b], (long long)res.quot, (long long)res.rem);
			pc_check(res.rem == 0 || ((res.rem < 0) == (nums[a] < 0)),
			    "ldiv(%lld, %lld) gave remainder %lld, whose sign differs from the numerator's",
			    (long long)nums[a], (long long)dens[b], (long long)res.rem);
		}
	}
	pc_pass();
}
