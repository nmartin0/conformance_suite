/*
 * LIO_WAIT and LIO_NOWAIT have different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.aio.h.003
 */
#include "pc.h"
#include <aio.h>

int main(void)
{
	pc_begin("XBD.aio.h.003");
#if !defined(LIO_WAIT) || !defined(LIO_NOWAIT)
	pc_unresolved("a constant is missing (the header check reports it): LIO_WAIT LIO_NOWAIT");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "LIO_WAIT"; e[n++].v = (long long)(LIO_WAIT);
	e[n].n = "LIO_NOWAIT"; e[n++].v = (long long)(LIO_NOWAIT);
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
