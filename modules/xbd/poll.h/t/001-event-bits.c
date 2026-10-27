/*
 * All ten POLL event constants are nonzero and fit in a short (the type of
 * events and revents).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.poll.h.001
 */
#include "pc.h"
#include <poll.h>

int main(void)
{
	pc_begin("XBD.poll.h.001");
#if !defined(POLLIN) || !defined(POLLRDNORM) || !defined(POLLRDBAND) || !defined(POLLPRI) || !defined(POLLOUT) || !defined(POLLWRNORM) || !defined(POLLWRBAND) || !defined(POLLERR) || !defined(POLLHUP) || !defined(POLLNVAL)
	pc_unresolved("a constant is missing (the header check reports it): POLLIN POLLRDNORM POLLRDBAND POLLPRI POLLOUT POLLWRNORM POLLWRBAND POLLERR POLLHUP POLLNVAL");
#else
	{
	struct { const char *n; long long v; } e[11];
	int n = 0, i, j;

	e[n].n = "POLLIN"; e[n++].v = (long long)(POLLIN);
	e[n].n = "POLLRDNORM"; e[n++].v = (long long)(POLLRDNORM);
	e[n].n = "POLLRDBAND"; e[n++].v = (long long)(POLLRDBAND);
	e[n].n = "POLLPRI"; e[n++].v = (long long)(POLLPRI);
	e[n].n = "POLLOUT"; e[n++].v = (long long)(POLLOUT);
	e[n].n = "POLLWRNORM"; e[n++].v = (long long)(POLLWRNORM);
	e[n].n = "POLLWRBAND"; e[n++].v = (long long)(POLLWRBAND);
	e[n].n = "POLLERR"; e[n++].v = (long long)(POLLERR);
	e[n].n = "POLLHUP"; e[n++].v = (long long)(POLLHUP);
	e[n].n = "POLLNVAL"; e[n++].v = (long long)(POLLNVAL);
	if (n < 10)
		pc_unresolved("fewer than 10 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check(e[i].v != 0, "%s is zero", e[i].n);
	for (i = 0; i < n; i++)
		pc_check((long long)(short)e[i].v == e[i].v, "%s does not fit in a short", e[i].n);
	(void)j;
	pc_pass();
	}
#endif
}
