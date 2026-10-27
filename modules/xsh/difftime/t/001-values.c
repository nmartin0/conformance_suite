/*
 * difftime(time1, time0) returns time1 - time0 in seconds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.difftime.001
 */
#include "pc.h"
#include <time.h>

int main(void)
{
	static const struct { long a, b; double d; } tab[] = {
		{ 5, 3, 2.0 }, { 3, 5, -2.0 }, { 7, 7, 0.0 }, { 0, 0, 0.0 },
		{ 1700000000L, 1, 1699999999.0 }, { 2000000000L, 1000000000L, 1000000000.0 },
		{ 1, 1700000000L, -1699999999.0 }, { 86400, 0, 86400.0 },
	};
	size_t i;

	pc_begin("XSH.difftime.001");
	pc_test();
	for (i = 0; i < sizeof tab / sizeof tab[0]; i++) {
		double d = difftime((time_t)tab[i].a, (time_t)tab[i].b);

		pc_check(d == tab[i].d, "difftime(%ld, %ld) is %f, expected %f", tab[i].a, tab[i].b,
		    d, tab[i].d);
	}
	pc_pass();
}
