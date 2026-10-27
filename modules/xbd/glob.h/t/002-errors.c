/*
 * GLOB_ABORTED, GLOB_NOMATCH and GLOB_NOSPACE are nonzero and have three
 * different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.glob.h.002
 */
#include "pc.h"
#include <glob.h>

int main(void)
{
	pc_begin("XBD.glob.h.002");
#if !defined(GLOB_ABORTED) || !defined(GLOB_NOMATCH) || !defined(GLOB_NOSPACE)
	pc_unresolved("a constant is missing (the header check reports it): GLOB_ABORTED GLOB_NOMATCH GLOB_NOSPACE");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "GLOB_ABORTED"; e[n++].v = (long long)(GLOB_ABORTED);
	e[n].n = "GLOB_NOMATCH"; e[n++].v = (long long)(GLOB_NOMATCH);
	e[n].n = "GLOB_NOSPACE"; e[n++].v = (long long)(GLOB_NOSPACE);
	if (n < 3)
		pc_unresolved("fewer than 3 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check(e[i].v != 0, "%s is zero", e[i].n);
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
