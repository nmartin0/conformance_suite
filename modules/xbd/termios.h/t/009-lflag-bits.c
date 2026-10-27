/*
 * ECHO, ECHOE, ECHOK, ECHONL, ICANON, IEXTEN, ISIG, NOFLSH and TOSTOP are
 * nonzero and share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.009
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	pc_begin("XBD.termios.h.009");
#if !defined(ECHO) || !defined(ECHOE) || !defined(ECHOK) || !defined(ECHONL) || !defined(ICANON) || !defined(IEXTEN) || !defined(ISIG) || !defined(NOFLSH) || !defined(TOSTOP)
	pc_unresolved("a constant is missing (the header check reports it): ECHO ECHOE ECHOK ECHONL ICANON IEXTEN ISIG NOFLSH TOSTOP");
#else
	{
	struct { const char *n; long long v; } e[10];
	int n = 0, i, j;

	e[n].n = "ECHO"; e[n++].v = (long long)(ECHO);
	e[n].n = "ECHOE"; e[n++].v = (long long)(ECHOE);
	e[n].n = "ECHOK"; e[n++].v = (long long)(ECHOK);
	e[n].n = "ECHONL"; e[n++].v = (long long)(ECHONL);
	e[n].n = "ICANON"; e[n++].v = (long long)(ICANON);
	e[n].n = "IEXTEN"; e[n++].v = (long long)(IEXTEN);
	e[n].n = "ISIG"; e[n++].v = (long long)(ISIG);
	e[n].n = "NOFLSH"; e[n++].v = (long long)(NOFLSH);
	e[n].n = "TOSTOP"; e[n++].v = (long long)(TOSTOP);
	if (n < 9)
		pc_unresolved("fewer than 9 of the constants are defined");
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
