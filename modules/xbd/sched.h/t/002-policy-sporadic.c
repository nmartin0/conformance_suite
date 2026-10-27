/*
 * SCHED_SPORADIC differs from SCHED_FIFO, SCHED_RR and SCHED_OTHER (as far
 * as they are defined).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sched.h.002
 */
#include "pc.h"
#include <sched.h>

int main(void)
{
	pc_begin("XBD.sched.h.002");
#if !defined(SCHED_SPORADIC)
	pc_unresolved("a constant is missing (the header check reports it): SCHED_SPORADIC");
#else
	{
	struct { const char *n; long long v; } e[5];
	int n = 0, i, j;

	e[n].n = "SCHED_SPORADIC"; e[n++].v = (long long)(SCHED_SPORADIC);
#ifdef SCHED_FIFO
	e[n].n = "SCHED_FIFO"; e[n++].v = (long long)(SCHED_FIFO);
#endif
#ifdef SCHED_RR
	e[n].n = "SCHED_RR"; e[n++].v = (long long)(SCHED_RR);
#endif
#ifdef SCHED_OTHER
	e[n].n = "SCHED_OTHER"; e[n++].v = (long long)(SCHED_OTHER);
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
