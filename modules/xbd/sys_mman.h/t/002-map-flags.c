/*
 * MAP_SHARED, MAP_PRIVATE, MAP_FIXED and MAP_ANONYMOUS are nonzero and
 * share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.002
 */
#include "pc.h"
#include <sys/mman.h>

int main(void)
{
	pc_begin("XBD.sys_mman.h.002");
#if !defined(MAP_SHARED) || !defined(MAP_PRIVATE) || !defined(MAP_FIXED) || !defined(MAP_ANONYMOUS)
	pc_unresolved("a constant is missing (the header check reports it): MAP_SHARED MAP_PRIVATE MAP_FIXED MAP_ANONYMOUS");
#else
	{
	struct { const char *n; long long v; } e[5];
	int n = 0, i, j;

	e[n].n = "MAP_SHARED"; e[n++].v = (long long)(MAP_SHARED);
	e[n].n = "MAP_PRIVATE"; e[n++].v = (long long)(MAP_PRIVATE);
	e[n].n = "MAP_FIXED"; e[n++].v = (long long)(MAP_FIXED);
	e[n].n = "MAP_ANONYMOUS"; e[n++].v = (long long)(MAP_ANONYMOUS);
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
