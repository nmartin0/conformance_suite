/*
 * PTHREAD_CANCEL_ENABLE and PTHREAD_CANCEL_DISABLE have different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.001
 */
#include "pc.h"
#include <pthread.h>

int main(void)
{
	pc_begin("XBD.pthread.h.001");
#if !defined(PTHREAD_CANCEL_ENABLE) || !defined(PTHREAD_CANCEL_DISABLE)
	pc_unresolved("a constant is missing (the header check reports it): PTHREAD_CANCEL_ENABLE PTHREAD_CANCEL_DISABLE");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "PTHREAD_CANCEL_ENABLE"; e[n++].v = (long long)(PTHREAD_CANCEL_ENABLE);
	e[n].n = "PTHREAD_CANCEL_DISABLE"; e[n++].v = (long long)(PTHREAD_CANCEL_DISABLE);
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
