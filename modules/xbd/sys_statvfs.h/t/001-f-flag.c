/*
 * ST_RDONLY and ST_NOSUID are nonzero and share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_statvfs.h.001
 */
#include "pc.h"
#include <sys/statvfs.h>

int main(void)
{
	pc_begin("XBD.sys_statvfs.h.001");
#if !defined(ST_RDONLY) || !defined(ST_NOSUID)
	pc_unresolved("a constant is missing (the header check reports it): ST_RDONLY ST_NOSUID");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "ST_RDONLY"; e[n++].v = (long long)(ST_RDONLY);
	e[n].n = "ST_NOSUID"; e[n++].v = (long long)(ST_NOSUID);
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
