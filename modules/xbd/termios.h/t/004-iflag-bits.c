/*
 * BRKINT, ICRNL, IGNBRK, IGNCR, IGNPAR, INLCR, INPCK, ISTRIP, IXANY,
 * IXOFF, IXON and PARMRK are nonzero and share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.004
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	pc_begin("XBD.termios.h.004");
#if !defined(BRKINT) || !defined(ICRNL) || !defined(IGNBRK) || !defined(IGNCR) || !defined(IGNPAR) || !defined(INLCR) || !defined(INPCK) || !defined(ISTRIP) || !defined(IXANY) || !defined(IXOFF) || !defined(IXON) || !defined(PARMRK)
	pc_unresolved("a constant is missing (the header check reports it): BRKINT ICRNL IGNBRK IGNCR IGNPAR INLCR INPCK ISTRIP IXANY IXOFF IXON PARMRK");
#else
	{
	struct { const char *n; long long v; } e[13];
	int n = 0, i, j;

	e[n].n = "BRKINT"; e[n++].v = (long long)(BRKINT);
	e[n].n = "ICRNL"; e[n++].v = (long long)(ICRNL);
	e[n].n = "IGNBRK"; e[n++].v = (long long)(IGNBRK);
	e[n].n = "IGNCR"; e[n++].v = (long long)(IGNCR);
	e[n].n = "IGNPAR"; e[n++].v = (long long)(IGNPAR);
	e[n].n = "INLCR"; e[n++].v = (long long)(INLCR);
	e[n].n = "INPCK"; e[n++].v = (long long)(INPCK);
	e[n].n = "ISTRIP"; e[n++].v = (long long)(ISTRIP);
	e[n].n = "IXANY"; e[n++].v = (long long)(IXANY);
	e[n].n = "IXOFF"; e[n++].v = (long long)(IXOFF);
	e[n].n = "IXON"; e[n++].v = (long long)(IXON);
	e[n].n = "PARMRK"; e[n++].v = (long long)(PARMRK);
	if (n < 12)
		pc_unresolved("fewer than 12 of the constants are defined");
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
