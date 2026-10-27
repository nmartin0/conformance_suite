/*
 * On XSI systems RUSAGE_SELF and RUSAGE_CHILDREN have different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_resource.h.002
 */
#include "pc.h"
#include <sys/resource.h>

int main(void)
{
	pc_begin("XBD.sys_resource.h.002");
#if !defined(RUSAGE_SELF) || !defined(RUSAGE_CHILDREN)
	pc_unresolved("a constant is missing (the header check reports it): RUSAGE_SELF RUSAGE_CHILDREN");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "RUSAGE_SELF"; e[n++].v = (long long)(RUSAGE_SELF);
	e[n].n = "RUSAGE_CHILDREN"; e[n++].v = (long long)(RUSAGE_CHILDREN);
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
