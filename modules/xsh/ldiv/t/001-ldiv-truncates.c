/*
 * ldiv() truncates an inexact quotient toward zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldiv.001
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.ldiv.001");
	pc_test();
	static const long num[] = { 7L, -7L, 7L, -7L, 1L, -1L, 5L, -5L, 100L, -100L };
	static const long den[] = { 2L, 2L, -2L, -2L, 3L, 3L, 7L, 7L, 7L, 7L };
	static const long quo[] = { 3L, -3L, -3L, 3L, 0L, 0L, 0L, 0L, 14L, -14L };
	static const long rem[] = { 1L, -1L, 1L, -1L, 1L, -1L, 5L, -5L, 2L, -2L };
	volatile long n, d;
	ldiv_t res;
	int k;

	for (k = 0; k < 10; k++) {
		n = num[k];
		d = den[k];
		res = ldiv(n, d);
		pc_check(res.quot == quo[k] && res.rem == rem[k], "ldiv(%ld, %ld) gave quot %ld rem %ld, expected %ld and %ld",
		    num[k], den[k], res.quot, res.rem, quo[k], rem[k]);
	}
	pc_pass();
}
