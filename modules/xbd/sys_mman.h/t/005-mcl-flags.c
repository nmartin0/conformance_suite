/*
 * MCL_CURRENT and MCL_FUTURE are nonzero and share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.005
 */
#include "pc.h"
#include <sys/mman.h>

int main(void)
{
	pc_begin("XBD.sys_mman.h.005");
#if !defined(MCL_CURRENT) || !defined(MCL_FUTURE)
	pc_unresolved("a constant is missing (the header check reports it): MCL_CURRENT MCL_FUTURE");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "MCL_CURRENT"; e[n++].v = (long long)(MCL_CURRENT);
	e[n].n = "MCL_FUTURE"; e[n++].v = (long long)(MCL_FUTURE);
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
