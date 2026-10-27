/*
 * LIO_NOP, LIO_READ and LIO_WRITE have three different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.aio.h.002
 */
#include "pc.h"
#include <aio.h>

int main(void)
{
	pc_begin("XBD.aio.h.002");
#if !defined(LIO_NOP) || !defined(LIO_READ) || !defined(LIO_WRITE)
	pc_unresolved("a constant is missing (the header check reports it): LIO_NOP LIO_READ LIO_WRITE");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "LIO_NOP"; e[n++].v = (long long)(LIO_NOP);
	e[n].n = "LIO_READ"; e[n++].v = (long long)(LIO_READ);
	e[n].n = "LIO_WRITE"; e[n++].v = (long long)(LIO_WRITE);
	if (n < 3)
		pc_unresolved("fewer than 3 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
