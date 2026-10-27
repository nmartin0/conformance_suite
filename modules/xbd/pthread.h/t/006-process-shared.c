/*
 * PTHREAD_PROCESS_SHARED and PTHREAD_PROCESS_PRIVATE have different
 * values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.006
 */
#include "pc.h"
#include <pthread.h>

int main(void)
{
	pc_begin("XBD.pthread.h.006");
#if !defined(PTHREAD_PROCESS_SHARED) || !defined(PTHREAD_PROCESS_PRIVATE)
	pc_unresolved("a constant is missing (the header check reports it): PTHREAD_PROCESS_SHARED PTHREAD_PROCESS_PRIVATE");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "PTHREAD_PROCESS_SHARED"; e[n++].v = (long long)(PTHREAD_PROCESS_SHARED);
	e[n].n = "PTHREAD_PROCESS_PRIVATE"; e[n++].v = (long long)(PTHREAD_PROCESS_PRIVATE);
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
