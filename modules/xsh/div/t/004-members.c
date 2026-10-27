/*
 * div_t has int members quot and rem.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.div.004
 */
#include "pc.h"
#include <limits.h>
#include <stdlib.h>

int main(void)
{
	pc_begin("XSH.div.004");
	pc_test();
	volatile int n = 7, d = 2;
	div_t res = div(n, d);

	pc_check(_Generic(res.quot, int: 1, default: 0), "div_t quot member has the wrong type");
	pc_check(_Generic(res.rem, int: 1, default: 0), "div_t rem member has the wrong type");
	pc_check(res.quot == 3 && res.rem == 1, "div(7, 2) gave quot and rem that do not match");
	pc_pass();
}
