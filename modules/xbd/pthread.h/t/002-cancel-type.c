/*
 * PTHREAD_CANCEL_DEFERRED and PTHREAD_CANCEL_ASYNCHRONOUS have different
 * values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.002
 */
#include "pc.h"
#include <pthread.h>

int main(void)
{
	pc_begin("XBD.pthread.h.002");
#if !defined(PTHREAD_CANCEL_DEFERRED) || !defined(PTHREAD_CANCEL_ASYNCHRONOUS)
	pc_unresolved("a constant is missing (the header check reports it): PTHREAD_CANCEL_DEFERRED PTHREAD_CANCEL_ASYNCHRONOUS");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "PTHREAD_CANCEL_DEFERRED"; e[n++].v = (long long)(PTHREAD_CANCEL_DEFERRED);
	e[n].n = "PTHREAD_CANCEL_ASYNCHRONOUS"; e[n++].v = (long long)(PTHREAD_CANCEL_ASYNCHRONOUS);
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
