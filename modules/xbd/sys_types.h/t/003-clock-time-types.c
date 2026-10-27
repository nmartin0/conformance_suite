/*
 * clock_t is real, time_t is an integer type at least 64 bits wide.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_types.h.003
 */
#include <sys/types.h>
#include <limits.h>
#include "pc.h"

_Static_assert(_Generic((clock_t)0, float _Complex: 0, double _Complex: 0, long double _Complex: 0, default: 1), "clock_t is a complex type");
_Static_assert((time_t)7 % (time_t)3 == 1, "time_t is not an integer type");

int main(void)
{
	pc_begin("XBD.sys_types.h.003");
	pc_test();
	clock_t c = 3;
	time_t t = (time_t)(1LL << 62);

	c = c * 2 + 1;
	pc_check(c == 7, "clock_t does not behave as a number");
	pc_check((long long)t == (1LL << 62) && t > 0, "time_t does not hold 2^62");
	t = (time_t)((1LL << 62) + 12345);
	pc_check((long long)t == (1LL << 62) + 12345, "time_t does not hold 2^62 + 12345");
	t = (time_t)LLONG_MAX;
	pc_check((long long)t == LLONG_MAX && t > 0, "time_t does not hold 2^63 - 1");
	pc_pass();
}
