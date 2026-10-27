/*
 * div() truncates an inexact quotient toward zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.div.001
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.div.001");
	pc_test();
	static const int num[] = { 7, -7, 7, -7, 1, -1, 5, -5, 100, -100 };
	static const int den[] = { 2, 2, -2, -2, 3, 3, 7, 7, 7, 7 };
	static const int quo[] = { 3, -3, -3, 3, 0, 0, 0, 0, 14, -14 };
	static const int rem[] = { 1, -1, 1, -1, 1, -1, 5, -5, 2, -2 };
	volatile int n, d;
	div_t res;
	int k;

	for (k = 0; k < 10; k++) {
		n = num[k];
		d = den[k];
		res = div(n, d);
		pc_check(res.quot == quo[k] && res.rem == rem[k], "div(%d, %d) gave quot %d rem %d, expected %d and %d",
		    num[k], den[k], res.quot, res.rem, quo[k], rem[k]);
	}
	pc_pass();
}
