/*
 * PTHREAD_EXPLICIT_SCHED and PTHREAD_INHERIT_SCHED have different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.007
 */
#include "pc.h"
#include <pthread.h>

int main(void)
{
	pc_begin("XBD.pthread.h.007");
#if !defined(PTHREAD_EXPLICIT_SCHED) || !defined(PTHREAD_INHERIT_SCHED)
	pc_unresolved("a constant is missing (the header check reports it): PTHREAD_EXPLICIT_SCHED PTHREAD_INHERIT_SCHED");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "PTHREAD_EXPLICIT_SCHED"; e[n++].v = (long long)(PTHREAD_EXPLICIT_SCHED);
	e[n].n = "PTHREAD_INHERIT_SCHED"; e[n++].v = (long long)(PTHREAD_INHERIT_SCHED);
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
