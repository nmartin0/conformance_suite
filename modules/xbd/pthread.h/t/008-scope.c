/*
 * PTHREAD_SCOPE_PROCESS and PTHREAD_SCOPE_SYSTEM have different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.008
 */
#include "pc.h"
#include <pthread.h>

int main(void)
{
	pc_begin("XBD.pthread.h.008");
#if !defined(PTHREAD_SCOPE_PROCESS) || !defined(PTHREAD_SCOPE_SYSTEM)
	pc_unresolved("a constant is missing (the header check reports it): PTHREAD_SCOPE_PROCESS PTHREAD_SCOPE_SYSTEM");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "PTHREAD_SCOPE_PROCESS"; e[n++].v = (long long)(PTHREAD_SCOPE_PROCESS);
	e[n].n = "PTHREAD_SCOPE_SYSTEM"; e[n++].v = (long long)(PTHREAD_SCOPE_SYSTEM);
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
