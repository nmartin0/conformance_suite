/*
 * lldiv_t has long long members quot and rem.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldiv.007
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.ldiv.007");
	pc_test();
	volatile long long n = 7, d = 2;
	lldiv_t res = lldiv(n, d);

	pc_check(_Generic(res.quot, long long: 1, default: 0), "lldiv_t quot member has the wrong type");
	pc_check(_Generic(res.rem, long long: 1, default: 0), "lldiv_t rem member has the wrong type");
	pc_check(res.quot == 3 && res.rem == 1, "lldiv(7, 2) gave quot and rem that do not match");
	pc_pass();
}
