/*
 * difftime() returns a double and is antisymmetric.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.difftime.002
 */
#include "pc.h"
#include <time.h>

int main(void)
{
	time_t a = 1500000000, b = 1499990000;
	time_t now;
	double x, y;

	pc_begin("XSH.difftime.002");
	pc_test();
	pc_check(_Generic(difftime(a, b), double: 1, default: 0), "difftime does not return double");
	x = difftime(a, b);
	y = difftime(b, a);
	pc_check(x == 10000.0, "difftime is %f, expected 10000", x);
	pc_check(y == -x, "difftime(b, a) is %f, not the negation of %f", y, x);
	pc_check(difftime(a, a) == 0.0, "difftime(a, a) is not 0");
	now = time(NULL);
	pc_check(now != (time_t)-1, "time failed");
	pc_check(difftime(now, now - 3600) == 3600.0, "difftime over an hour is not 3600");
	pc_pass();
}
