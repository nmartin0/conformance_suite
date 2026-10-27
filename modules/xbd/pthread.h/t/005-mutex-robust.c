/*
 * PTHREAD_MUTEX_ROBUST and PTHREAD_MUTEX_STALLED have different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.005
 */
#include "pc.h"
#include <pthread.h>

int main(void)
{
	pc_begin("XBD.pthread.h.005");
#if !defined(PTHREAD_MUTEX_ROBUST) || !defined(PTHREAD_MUTEX_STALLED)
	pc_unresolved("a constant is missing (the header check reports it): PTHREAD_MUTEX_ROBUST PTHREAD_MUTEX_STALLED");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "PTHREAD_MUTEX_ROBUST"; e[n++].v = (long long)(PTHREAD_MUTEX_ROBUST);
	e[n].n = "PTHREAD_MUTEX_STALLED"; e[n++].v = (long long)(PTHREAD_MUTEX_STALLED);
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
