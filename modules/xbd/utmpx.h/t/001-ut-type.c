/*
 * EMPTY, BOOT_TIME, OLD_TIME, NEW_TIME, USER_PROCESS, INIT_PROCESS,
 * LOGIN_PROCESS and DEAD_PROCESS have eight different values that fit in
 * ut_type (a short).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.utmpx.h.001
 */
#include "pc.h"
#include <utmpx.h>

int main(void)
{
	pc_begin("XBD.utmpx.h.001");
#if !defined(EMPTY) || !defined(BOOT_TIME) || !defined(OLD_TIME) || !defined(NEW_TIME) || !defined(USER_PROCESS) || !defined(INIT_PROCESS) || !defined(LOGIN_PROCESS) || !defined(DEAD_PROCESS)
	pc_unresolved("a constant is missing (the header check reports it): EMPTY BOOT_TIME OLD_TIME NEW_TIME USER_PROCESS INIT_PROCESS LOGIN_PROCESS DEAD_PROCESS");
#else
	{
	struct { const char *n; long long v; } e[9];
	int n = 0, i, j;

	e[n].n = "EMPTY"; e[n++].v = (long long)(EMPTY);
	e[n].n = "BOOT_TIME"; e[n++].v = (long long)(BOOT_TIME);
	e[n].n = "OLD_TIME"; e[n++].v = (long long)(OLD_TIME);
	e[n].n = "NEW_TIME"; e[n++].v = (long long)(NEW_TIME);
	e[n].n = "USER_PROCESS"; e[n++].v = (long long)(USER_PROCESS);
	e[n].n = "INIT_PROCESS"; e[n++].v = (long long)(INIT_PROCESS);
	e[n].n = "LOGIN_PROCESS"; e[n++].v = (long long)(LOGIN_PROCESS);
	e[n].n = "DEAD_PROCESS"; e[n++].v = (long long)(DEAD_PROCESS);
	if (n < 8)
		pc_unresolved("fewer than 8 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check((long long)(short)e[i].v == e[i].v, "%s does not fit in a short", e[i].n);
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
