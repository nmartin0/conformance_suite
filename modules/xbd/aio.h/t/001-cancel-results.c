/*
 * AIO_ALLDONE, AIO_CANCELED and AIO_NOTCANCELED have three different
 * values and none is -1, the error return of aio_cancel().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.aio.h.001
 */
#include "pc.h"
#include <aio.h>

int main(void)
{
	pc_begin("XBD.aio.h.001");
#if !defined(AIO_ALLDONE) || !defined(AIO_CANCELED) || !defined(AIO_NOTCANCELED)
	pc_unresolved("a constant is missing (the header check reports it): AIO_ALLDONE AIO_CANCELED AIO_NOTCANCELED");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "AIO_ALLDONE"; e[n++].v = (long long)(AIO_ALLDONE);
	e[n].n = "AIO_CANCELED"; e[n++].v = (long long)(AIO_CANCELED);
	e[n].n = "AIO_NOTCANCELED"; e[n++].v = (long long)(AIO_NOTCANCELED);
	if (n < 3)
		pc_unresolved("fewer than 3 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	for (i = 0; i < n; i++)
		pc_check(e[i].v != -1, "%s equals -1, which aio_cancel() returns for an error", e[i].n);
	pc_pass();
	}
#endif
}
