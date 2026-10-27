/*
 * CLOCAL, CREAD, CSIZE, CSTOPB, HUPCL, PARENB and PARODD are nonzero and
 * share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.007
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	pc_begin("XBD.termios.h.007");
#if !defined(CLOCAL) || !defined(CREAD) || !defined(CSIZE) || !defined(CSTOPB) || !defined(HUPCL) || !defined(PARENB) || !defined(PARODD)
	pc_unresolved("a constant is missing (the header check reports it): CLOCAL CREAD CSIZE CSTOPB HUPCL PARENB PARODD");
#else
	{
	struct { const char *n; long long v; } e[8];
	int n = 0, i, j;

	e[n].n = "CLOCAL"; e[n++].v = (long long)(CLOCAL);
	e[n].n = "CREAD"; e[n++].v = (long long)(CREAD);
	e[n].n = "CSIZE"; e[n++].v = (long long)(CSIZE);
	e[n].n = "CSTOPB"; e[n++].v = (long long)(CSTOPB);
	e[n].n = "HUPCL"; e[n++].v = (long long)(HUPCL);
	e[n].n = "PARENB"; e[n++].v = (long long)(PARENB);
	e[n].n = "PARODD"; e[n++].v = (long long)(PARODD);
	if (n < 7)
		pc_unresolved("fewer than 7 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check(e[i].v != 0, "%s is zero", e[i].n);
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(((unsigned long long)e[i].v & (unsigned long long)e[j].v) == 0, "%s (%#llx) and %s (%#llx) have bits in common", e[i].n, (unsigned long long)e[i].v, e[j].n, (unsigned long long)e[j].v);
		}
	pc_pass();
	}
#endif
}
