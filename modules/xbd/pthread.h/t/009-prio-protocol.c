/*
 * PTHREAD_PRIO_NONE, PTHREAD_PRIO_INHERIT and PTHREAD_PRIO_PROTECT, as far
 * as defined, have different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.009
 */
#include "pc.h"
#include <pthread.h>

int main(void)
{
	pc_begin("XBD.pthread.h.009");
#if !defined(PTHREAD_PRIO_NONE)
	pc_unresolved("a constant is missing (the header check reports it): PTHREAD_PRIO_NONE");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "PTHREAD_PRIO_NONE"; e[n++].v = (long long)(PTHREAD_PRIO_NONE);
#ifdef PTHREAD_PRIO_INHERIT
	e[n].n = "PTHREAD_PRIO_INHERIT"; e[n++].v = (long long)(PTHREAD_PRIO_INHERIT);
#endif
#ifdef PTHREAD_PRIO_PROTECT
	e[n].n = "PTHREAD_PRIO_PROTECT"; e[n++].v = (long long)(PTHREAD_PRIO_PROTECT);
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
