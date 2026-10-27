/*
 * WRDE_BADCHAR, WRDE_BADVAL, WRDE_CMDSUB, WRDE_NOSPACE and WRDE_SYNTAX are
 * nonzero and have five different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wordexp.h.002
 */
#include "pc.h"
#include <wordexp.h>

int main(void)
{
	pc_begin("XBD.wordexp.h.002");
#if !defined(WRDE_BADCHAR) || !defined(WRDE_BADVAL) || !defined(WRDE_CMDSUB) || !defined(WRDE_NOSPACE) || !defined(WRDE_SYNTAX)
	pc_unresolved("a constant is missing (the header check reports it): WRDE_BADCHAR WRDE_BADVAL WRDE_CMDSUB WRDE_NOSPACE WRDE_SYNTAX");
#else
	{
	struct { const char *n; long long v; } e[6];
	int n = 0, i, j;

	e[n].n = "WRDE_BADCHAR"; e[n++].v = (long long)(WRDE_BADCHAR);
	e[n].n = "WRDE_BADVAL"; e[n++].v = (long long)(WRDE_BADVAL);
	e[n].n = "WRDE_CMDSUB"; e[n++].v = (long long)(WRDE_CMDSUB);
	e[n].n = "WRDE_NOSPACE"; e[n++].v = (long long)(WRDE_NOSPACE);
	e[n].n = "WRDE_SYNTAX"; e[n++].v = (long long)(WRDE_SYNTAX);
	if (n < 5)
		pc_unresolved("fewer than 5 of the constants are defined");
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
