/*
 * RLIMIT_CORE, RLIMIT_DATA, RLIMIT_FSIZE, RLIMIT_NOFILE, RLIMIT_STACK and
 * RLIMIT_AS (and RLIMIT_CPU on XSI systems) have different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_resource.h.003
 */
#include "pc.h"
#include <sys/resource.h>

int main(void)
{
	pc_begin("XBD.sys_resource.h.003");
#if !defined(RLIMIT_CORE) || !defined(RLIMIT_DATA) || !defined(RLIMIT_FSIZE) || !defined(RLIMIT_NOFILE) || !defined(RLIMIT_STACK) || !defined(RLIMIT_AS)
	pc_unresolved("a constant is missing (the header check reports it): RLIMIT_CORE RLIMIT_DATA RLIMIT_FSIZE RLIMIT_NOFILE RLIMIT_STACK RLIMIT_AS");
#else
	{
	struct { const char *n; long long v; } e[8];
	int n = 0, i, j;

	e[n].n = "RLIMIT_CORE"; e[n++].v = (long long)(RLIMIT_CORE);
	e[n].n = "RLIMIT_DATA"; e[n++].v = (long long)(RLIMIT_DATA);
	e[n].n = "RLIMIT_FSIZE"; e[n++].v = (long long)(RLIMIT_FSIZE);
	e[n].n = "RLIMIT_NOFILE"; e[n++].v = (long long)(RLIMIT_NOFILE);
	e[n].n = "RLIMIT_STACK"; e[n++].v = (long long)(RLIMIT_STACK);
	e[n].n = "RLIMIT_AS"; e[n++].v = (long long)(RLIMIT_AS);
#ifdef RLIMIT_CPU
	e[n].n = "RLIMIT_CPU"; e[n++].v = (long long)(RLIMIT_CPU);
#endif
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
