/*
 * ldiv_t has long members quot and rem.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ldiv.004
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.ldiv.004");
	pc_test();
	volatile long n = 7, d = 2;
	ldiv_t res = ldiv(n, d);

	pc_check(_Generic(res.quot, long: 1, default: 0), "ldiv_t quot member has the wrong type");
	pc_check(_Generic(res.rem, long: 1, default: 0), "ldiv_t rem member has the wrong type");
	pc_check(res.quot == 3 && res.rem == 1, "ldiv(7, 2) gave quot and rem that do not match");
	pc_pass();
}
