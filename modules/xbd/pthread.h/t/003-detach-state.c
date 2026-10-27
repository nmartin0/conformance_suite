/*
 * PTHREAD_CREATE_DETACHED and PTHREAD_CREATE_JOINABLE have different
 * values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.003
 */
#include "pc.h"
#include <pthread.h>

int main(void)
{
	pc_begin("XBD.pthread.h.003");
#if !defined(PTHREAD_CREATE_DETACHED) || !defined(PTHREAD_CREATE_JOINABLE)
	pc_unresolved("a constant is missing (the header check reports it): PTHREAD_CREATE_DETACHED PTHREAD_CREATE_JOINABLE");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "PTHREAD_CREATE_DETACHED"; e[n++].v = (long long)(PTHREAD_CREATE_DETACHED);
	e[n].n = "PTHREAD_CREATE_JOINABLE"; e[n++].v = (long long)(PTHREAD_CREATE_JOINABLE);
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
