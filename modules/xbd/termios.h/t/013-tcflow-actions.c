/*
 * TCOOFF, TCOON, TCIOFF and TCION have four different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.013
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	pc_begin("XBD.termios.h.013");
#if !defined(TCOOFF) || !defined(TCOON) || !defined(TCIOFF) || !defined(TCION)
	pc_unresolved("a constant is missing (the header check reports it): TCOOFF TCOON TCIOFF TCION");
#else
	{
	struct { const char *n; long long v; } e[5];
	int n = 0, i, j;

	e[n].n = "TCOOFF"; e[n++].v = (long long)(TCOOFF);
	e[n].n = "TCOON"; e[n++].v = (long long)(TCOON);
	e[n].n = "TCIOFF"; e[n++].v = (long long)(TCIOFF);
	e[n].n = "TCION"; e[n++].v = (long long)(TCION);
	if (n < 4)
		pc_unresolved("fewer than 4 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
