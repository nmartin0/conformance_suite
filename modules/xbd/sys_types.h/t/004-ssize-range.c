/*
 * ssize_t holds -1 and SSIZE_MAX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_types.h.004
 */
#include <sys/types.h>
#include <limits.h>
#include "pc.h"

_Static_assert((ssize_t)-1 < 0, "ssize_t is not signed");
_Static_assert(SSIZE_MAX > 0, "SSIZE_MAX is not positive");

int main(void)
{
	pc_begin("XBD.sys_types.h.004");
	pc_test();
	ssize_t a = -1, b = SSIZE_MAX, c = SSIZE_MAX - 1;

	pc_check(a == -1 && b == SSIZE_MAX && c == SSIZE_MAX - 1, "ssize_t does not hold -1, SSIZE_MAX and SSIZE_MAX - 1");
	pc_check(b > 0 && c > 0 && b > c, "SSIZE_MAX is not held as a positive value");
	pc_pass();
}
