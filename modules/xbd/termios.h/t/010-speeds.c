/*
 * B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400,
 * B4800, B9600, B19200 and B38400 have sixteen different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.010
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	pc_begin("XBD.termios.h.010");
#if !defined(B0) || !defined(B50) || !defined(B75) || !defined(B110) || !defined(B134) || !defined(B150) || !defined(B200) || !defined(B300) || !defined(B600) || !defined(B1200) || !defined(B1800) || !defined(B2400) || !defined(B4800) || !defined(B9600) || !defined(B19200) || !defined(B38400)
	pc_unresolved("a constant is missing (the header check reports it): B0 B50 B75 B110 B134 B150 B200 B300 B600 B1200 B1800 B2400 B4800 B9600 B19200 B38400");
#else
	{
	struct { const char *n; long long v; } e[17];
	int n = 0, i, j;

	e[n].n = "B0"; e[n++].v = (long long)(B0);
	e[n].n = "B50"; e[n++].v = (long long)(B50);
	e[n].n = "B75"; e[n++].v = (long long)(B75);
	e[n].n = "B110"; e[n++].v = (long long)(B110);
	e[n].n = "B134"; e[n++].v = (long long)(B134);
	e[n].n = "B150"; e[n++].v = (long long)(B150);
	e[n].n = "B200"; e[n++].v = (long long)(B200);
	e[n].n = "B300"; e[n++].v = (long long)(B300);
	e[n].n = "B600"; e[n++].v = (long long)(B600);
	e[n].n = "B1200"; e[n++].v = (long long)(B1200);
	e[n].n = "B1800"; e[n++].v = (long long)(B1800);
	e[n].n = "B2400"; e[n++].v = (long long)(B2400);
	e[n].n = "B4800"; e[n++].v = (long long)(B4800);
	e[n].n = "B9600"; e[n++].v = (long long)(B9600);
	e[n].n = "B19200"; e[n++].v = (long long)(B19200);
	e[n].n = "B38400"; e[n++].v = (long long)(B38400);
	if (n < 16)
		pc_unresolved("fewer than 16 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
