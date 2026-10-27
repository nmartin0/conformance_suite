/*
 * TCSANOW, TCSADRAIN and TCSAFLUSH have three different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.011
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	pc_begin("XBD.termios.h.011");
#if !defined(TCSANOW) || !defined(TCSADRAIN) || !defined(TCSAFLUSH)
	pc_unresolved("a constant is missing (the header check reports it): TCSANOW TCSADRAIN TCSAFLUSH");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "TCSANOW"; e[n++].v = (long long)(TCSANOW);
	e[n].n = "TCSADRAIN"; e[n++].v = (long long)(TCSADRAIN);
	e[n].n = "TCSAFLUSH"; e[n++].v = (long long)(TCSAFLUSH);
	if (n < 3)
		pc_unresolved("fewer than 3 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
