/*
 * suseconds_t range and key_t (XSI).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_types.h.005
 */
#include <sys/types.h>
#include "pc.h"

_Static_assert((suseconds_t)-1 < 0, "suseconds_t is not signed");
_Static_assert((suseconds_t)7 % (suseconds_t)3 == 1, "suseconds_t is not an integer type");

int main(void)
{
	pc_begin("XBD.sys_types.h.005");
	pc_test();
	suseconds_t a = -1, b = 1000000, c = 999999;
	key_t k = 3;

	pc_check(a == -1 && b == 1000000 && c == 999999, "suseconds_t does not hold -1, 1000000 and 999999");
	k = k * 2 + 1;
	pc_check(k == 7 && k * 1.5 > 10.0, "key_t does not behave as an arithmetic type");
	pc_pass();
}
