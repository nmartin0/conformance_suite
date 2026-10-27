/*
 * FNM_PATHNAME, FNM_PERIOD, FNM_NOESCAPE and FNM_CASEFOLD are nonzero and
 * share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fnmatch.h.001
 */
#include "pc.h"
#include <fnmatch.h>

int main(void)
{
	pc_begin("XBD.fnmatch.h.001");
#if !defined(FNM_PATHNAME) || !defined(FNM_PERIOD) || !defined(FNM_NOESCAPE)
	pc_unresolved("a constant is missing (the header check reports it): FNM_PATHNAME FNM_PERIOD FNM_NOESCAPE");
#else
	{
	struct { const char *n; long long v; } e[5];
	int n = 0, i, j;

	e[n].n = "FNM_PATHNAME"; e[n++].v = (long long)(FNM_PATHNAME);
	e[n].n = "FNM_PERIOD"; e[n++].v = (long long)(FNM_PERIOD);
	e[n].n = "FNM_NOESCAPE"; e[n++].v = (long long)(FNM_NOESCAPE);
#ifdef FNM_CASEFOLD
	e[n].n = "FNM_CASEFOLD"; e[n++].v = (long long)(FNM_CASEFOLD);
#endif
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
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
