/*
 * FTW_PHYS, FTW_MOUNT, FTW_XDEV, FTW_DEPTH and FTW_CHDIR are nonzero and
 * share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.ftw.h.002
 */
#include "pc.h"
#include <ftw.h>

int main(void)
{
	pc_begin("XBD.ftw.h.002");
#if !defined(FTW_PHYS) || !defined(FTW_MOUNT) || !defined(FTW_XDEV) || !defined(FTW_DEPTH) || !defined(FTW_CHDIR)
	pc_unresolved("a constant is missing (the header check reports it): FTW_PHYS FTW_MOUNT FTW_XDEV FTW_DEPTH FTW_CHDIR");
#else
	{
	struct { const char *n; long long v; } e[6];
	int n = 0, i, j;

	e[n].n = "FTW_PHYS"; e[n++].v = (long long)(FTW_PHYS);
	e[n].n = "FTW_MOUNT"; e[n++].v = (long long)(FTW_MOUNT);
	e[n].n = "FTW_XDEV"; e[n++].v = (long long)(FTW_XDEV);
	e[n].n = "FTW_DEPTH"; e[n++].v = (long long)(FTW_DEPTH);
	e[n].n = "FTW_CHDIR"; e[n++].v = (long long)(FTW_CHDIR);
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
