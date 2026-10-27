/*
 * WRDE_APPEND, WRDE_DOOFFS, WRDE_NOCMD, WRDE_REUSE, WRDE_SHOWERR and
 * WRDE_UNDEF are nonzero and share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wordexp.h.001
 */
#include "pc.h"
#include <wordexp.h>

int main(void)
{
	pc_begin("XBD.wordexp.h.001");
#if !defined(WRDE_APPEND) || !defined(WRDE_DOOFFS) || !defined(WRDE_NOCMD) || !defined(WRDE_REUSE) || !defined(WRDE_SHOWERR) || !defined(WRDE_UNDEF)
	pc_unresolved("a constant is missing (the header check reports it): WRDE_APPEND WRDE_DOOFFS WRDE_NOCMD WRDE_REUSE WRDE_SHOWERR WRDE_UNDEF");
#else
	{
	struct { const char *n; long long v; } e[7];
	int n = 0, i, j;

	e[n].n = "WRDE_APPEND"; e[n++].v = (long long)(WRDE_APPEND);
	e[n].n = "WRDE_DOOFFS"; e[n++].v = (long long)(WRDE_DOOFFS);
	e[n].n = "WRDE_NOCMD"; e[n++].v = (long long)(WRDE_NOCMD);
	e[n].n = "WRDE_REUSE"; e[n++].v = (long long)(WRDE_REUSE);
	e[n].n = "WRDE_SHOWERR"; e[n++].v = (long long)(WRDE_SHOWERR);
	e[n].n = "WRDE_UNDEF"; e[n++].v = (long long)(WRDE_UNDEF);
	if (n < 6)
		pc_unresolved("fewer than 6 of the constants are defined");
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
