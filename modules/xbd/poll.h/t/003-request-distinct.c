/*
 * POLLIN, POLLRDNORM, POLLRDBAND, POLLPRI, POLLOUT and POLLWRBAND have six
 * different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.poll.h.003
 */
#include "pc.h"
#include <poll.h>

int main(void)
{
	pc_begin("XBD.poll.h.003");
#if !defined(POLLIN) || !defined(POLLRDNORM) || !defined(POLLRDBAND) || !defined(POLLPRI) || !defined(POLLOUT) || !defined(POLLWRBAND)
	pc_unresolved("a constant is missing (the header check reports it): POLLIN POLLRDNORM POLLRDBAND POLLPRI POLLOUT POLLWRBAND");
#else
	{
	struct { const char *n; long long v; } e[7];
	int n = 0, i, j;

	e[n].n = "POLLIN"; e[n++].v = (long long)(POLLIN);
	e[n].n = "POLLRDNORM"; e[n++].v = (long long)(POLLRDNORM);
	e[n].n = "POLLRDBAND"; e[n++].v = (long long)(POLLRDBAND);
	e[n].n = "POLLPRI"; e[n++].v = (long long)(POLLPRI);
	e[n].n = "POLLOUT"; e[n++].v = (long long)(POLLOUT);
	e[n].n = "POLLWRBAND"; e[n++].v = (long long)(POLLWRBAND);
	if (n < 6)
		pc_unresolved("fewer than 6 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
