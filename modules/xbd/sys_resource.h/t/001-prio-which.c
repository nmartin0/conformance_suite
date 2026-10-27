/*
 * On XSI systems PRIO_PROCESS, PRIO_PGRP and PRIO_USER have three
 * different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_resource.h.001
 */
#include "pc.h"
#include <sys/resource.h>

int main(void)
{
	pc_begin("XBD.sys_resource.h.001");
#if !defined(PRIO_PROCESS) || !defined(PRIO_PGRP) || !defined(PRIO_USER)
	pc_unresolved("a constant is missing (the header check reports it): PRIO_PROCESS PRIO_PGRP PRIO_USER");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "PRIO_PROCESS"; e[n++].v = (long long)(PRIO_PROCESS);
	e[n].n = "PRIO_PGRP"; e[n++].v = (long long)(PRIO_PGRP);
	e[n].n = "PRIO_USER"; e[n++].v = (long long)(PRIO_USER);
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
