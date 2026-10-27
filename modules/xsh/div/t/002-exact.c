/*
 * div() of an exact division has remainder 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.div.002
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.div.002");
	pc_test();
	static const int num[] = { 6, -6, 6, -6, 0, 0, 9, 9 };
	static const int den[] = { 3, 3, -3, -3, 5, -5, 1, 9 };
	static const int quo[] = { 2, -2, -2, 2, 0, 0, 9, 1 };
	static const int rem[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	volatile int n, d;
	div_t res;
	int k;

	for (k = 0; k < 8; k++) {
		n = num[k];
		d = den[k];
		res = div(n, d);
		pc_check(res.quot == quo[k] && res.rem == rem[k], "div(%d, %d) gave quot %d rem %d, expected %d and %d",
		    num[k], den[k], res.quot, res.rem, quo[k], rem[k]);
	}
	pc_pass();
}
