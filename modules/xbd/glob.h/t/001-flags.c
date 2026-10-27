/*
 * GLOB_APPEND, GLOB_DOOFFS, GLOB_ERR, GLOB_MARK, GLOB_NOCHECK,
 * GLOB_NOESCAPE and GLOB_NOSORT are nonzero and share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.glob.h.001
 */
#include "pc.h"
#include <glob.h>

int main(void)
{
	pc_begin("XBD.glob.h.001");
#if !defined(GLOB_APPEND) || !defined(GLOB_DOOFFS) || !defined(GLOB_ERR) || !defined(GLOB_MARK) || !defined(GLOB_NOCHECK) || !defined(GLOB_NOESCAPE) || !defined(GLOB_NOSORT)
	pc_unresolved("a constant is missing (the header check reports it): GLOB_APPEND GLOB_DOOFFS GLOB_ERR GLOB_MARK GLOB_NOCHECK GLOB_NOESCAPE GLOB_NOSORT");
#else
	{
	struct { const char *n; long long v; } e[8];
	int n = 0, i, j;

	e[n].n = "GLOB_APPEND"; e[n++].v = (long long)(GLOB_APPEND);
	e[n].n = "GLOB_DOOFFS"; e[n++].v = (long long)(GLOB_DOOFFS);
	e[n].n = "GLOB_ERR"; e[n++].v = (long long)(GLOB_ERR);
	e[n].n = "GLOB_MARK"; e[n++].v = (long long)(GLOB_MARK);
	e[n].n = "GLOB_NOCHECK"; e[n++].v = (long long)(GLOB_NOCHECK);
	e[n].n = "GLOB_NOESCAPE"; e[n++].v = (long long)(GLOB_NOESCAPE);
	e[n].n = "GLOB_NOSORT"; e[n++].v = (long long)(GLOB_NOSORT);
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
