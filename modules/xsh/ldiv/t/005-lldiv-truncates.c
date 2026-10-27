/*
 * lldiv() truncates an inexact quotient toward zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldiv.005
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.ldiv.005");
	pc_test();
	static const long long num[] = { 7LL, -7LL, 7LL, -7LL, 1LL, -1LL, 5LL, -5LL, 100LL, -100LL, 5000000000LL, -5000000000LL };
	static const long long den[] = { 2LL, 2LL, -2LL, -2LL, 3LL, 3LL, 7LL, 7LL, 7LL, 7LL, 3LL, 3LL };
	static const long long quo[] = { 3LL, -3LL, -3LL, 3LL, 0LL, 0LL, 0LL, 0LL, 14LL, -14LL, 1666666666LL, -1666666666LL };
	static const long long rem[] = { 1LL, -1LL, 1LL, -1LL, 1LL, -1LL, 5LL, -5LL, 2LL, -2LL, 2LL, -2LL };
	volatile long long n, d;
	lldiv_t res;
	int k;

	for (k = 0; k < 12; k++) {
		n = num[k];
		d = den[k];
		res = lldiv(n, d);
		pc_check(res.quot == quo[k] && res.rem == rem[k], "lldiv(%lld, %lld) gave quot %lld rem %lld, expected %lld and %lld",
		    num[k], den[k], res.quot, res.rem, quo[k], rem[k]);
	}
	pc_pass();
}
