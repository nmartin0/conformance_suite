/*
 * On XSI systems OPOST, ONLCR, OCRNL, ONOCR, ONLRET, OFILL, OFDEL, NLDLY,
 * CRDLY, TABDLY, BSDLY, VTDLY and FFDLY are nonzero and share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.005
 */
#include "pc.h"
#include <termios.h>

int main(void)
{
	pc_begin("XBD.termios.h.005");
#if !defined(OPOST) || !defined(ONLCR) || !defined(OCRNL) || !defined(ONOCR) || !defined(ONLRET) || !defined(OFILL) || !defined(OFDEL) || !defined(NLDLY) || !defined(CRDLY) || !defined(TABDLY) || !defined(BSDLY) || !defined(VTDLY) || !defined(FFDLY)
	pc_unresolved("a constant is missing (the header check reports it): OPOST ONLCR OCRNL ONOCR ONLRET OFILL OFDEL NLDLY CRDLY TABDLY BSDLY VTDLY FFDLY");
#else
	{
	struct { const char *n; long long v; } e[14];
	int n = 0, i, j;

	e[n].n = "OPOST"; e[n++].v = (long long)(OPOST);
	e[n].n = "ONLCR"; e[n++].v = (long long)(ONLCR);
	e[n].n = "OCRNL"; e[n++].v = (long long)(OCRNL);
	e[n].n = "ONOCR"; e[n++].v = (long long)(ONOCR);
	e[n].n = "ONLRET"; e[n++].v = (long long)(ONLRET);
	e[n].n = "OFILL"; e[n++].v = (long long)(OFILL);
	e[n].n = "OFDEL"; e[n++].v = (long long)(OFDEL);
	e[n].n = "NLDLY"; e[n++].v = (long long)(NLDLY);
	e[n].n = "CRDLY"; e[n++].v = (long long)(CRDLY);
	e[n].n = "TABDLY"; e[n++].v = (long long)(TABDLY);
	e[n].n = "BSDLY"; e[n++].v = (long long)(BSDLY);
	e[n].n = "VTDLY"; e[n++].v = (long long)(VTDLY);
	e[n].n = "FFDLY"; e[n++].v = (long long)(FFDLY);
	if (n < 13)
		pc_unresolved("fewer than 13 of the constants are defined");
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
