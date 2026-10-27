/*
 * SCHED_FIFO, SCHED_RR and SCHED_OTHER have three different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sched.h.001
 */
#include "pc.h"
#include <sched.h>

int main(void)
{
	pc_begin("XBD.sched.h.001");
#if !defined(SCHED_FIFO) || !defined(SCHED_RR) || !defined(SCHED_OTHER)
	pc_unresolved("a constant is missing (the header check reports it): SCHED_FIFO SCHED_RR SCHED_OTHER");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "SCHED_FIFO"; e[n++].v = (long long)(SCHED_FIFO);
	e[n].n = "SCHED_RR"; e[n++].v = (long long)(SCHED_RR);
	e[n].n = "SCHED_OTHER"; e[n++].v = (long long)(SCHED_OTHER);
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
