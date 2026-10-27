/*
 * ldiv() of an exact division has remainder 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldiv.002
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.ldiv.002");
	pc_test();
	static const long num[] = { 6L, -6L, 6L, -6L, 0L, 0L, 9L, 9L };
	static const long den[] = { 3L, 3L, -3L, -3L, 5L, -5L, 1L, 9L };
	static const long quo[] = { 2L, -2L, -2L, 2L, 0L, 0L, 9L, 1L };
	static const long rem[] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };
	volatile long n, d;
	ldiv_t res;
	int k;

	for (k = 0; k < 8; k++) {
		n = num[k];
		d = den[k];
		res = ldiv(n, d);
		pc_check(res.quot == quo[k] && res.rem == rem[k], "ldiv(%ld, %ld) gave quot %ld rem %ld, expected %ld and %ld",
		    num[k], den[k], res.quot, res.rem, quo[k], rem[k]);
	}
	pc_pass();
}
