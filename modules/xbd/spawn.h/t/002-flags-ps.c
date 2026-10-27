/*
 * POSIX_SPAWN_SETSCHEDPARAM and POSIX_SPAWN_SETSCHEDULER are nonzero, fit
 * in a short and share no bits with each other or with the five other
 * flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.spawn.h.002
 */
#include "pc.h"
#include <spawn.h>

int main(void)
{
	pc_begin("XBD.spawn.h.002");
#if !defined(POSIX_SPAWN_SETSCHEDPARAM) || !defined(POSIX_SPAWN_SETSCHEDULER)
	pc_unresolved("a constant is missing (the header check reports it): POSIX_SPAWN_SETSCHEDPARAM POSIX_SPAWN_SETSCHEDULER");
#else
	{
	struct { const char *n; long long v; } e[8];
	int n = 0, i, j;

	e[n].n = "POSIX_SPAWN_SETSCHEDPARAM"; e[n++].v = (long long)(POSIX_SPAWN_SETSCHEDPARAM);
	e[n].n = "POSIX_SPAWN_SETSCHEDULER"; e[n++].v = (long long)(POSIX_SPAWN_SETSCHEDULER);
#ifdef POSIX_SPAWN_RESETIDS
	e[n].n = "POSIX_SPAWN_RESETIDS"; e[n++].v = (long long)(POSIX_SPAWN_RESETIDS);
#endif
#ifdef POSIX_SPAWN_SETPGROUP
	e[n].n = "POSIX_SPAWN_SETPGROUP"; e[n++].v = (long long)(POSIX_SPAWN_SETPGROUP);
#endif
#ifdef POSIX_SPAWN_SETSIGDEF
	e[n].n = "POSIX_SPAWN_SETSIGDEF"; e[n++].v = (long long)(POSIX_SPAWN_SETSIGDEF);
#endif
#ifdef POSIX_SPAWN_SETSIGMASK
	e[n].n = "POSIX_SPAWN_SETSIGMASK"; e[n++].v = (long long)(POSIX_SPAWN_SETSIGMASK);
#endif
#ifdef POSIX_SPAWN_SETSID
	e[n].n = "POSIX_SPAWN_SETSID"; e[n++].v = (long long)(POSIX_SPAWN_SETSID);
#endif
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check(e[i].v != 0, "%s is zero", e[i].n);
	for (i = 0; i < n; i++)
		pc_check((long long)(short)e[i].v == e[i].v, "%s does not fit in a short", e[i].n);
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(((unsigned long long)e[i].v & (unsigned long long)e[j].v) == 0, "%s (%#llx) and %s (%#llx) have bits in common", e[i].n, (unsigned long long)e[i].v, e[j].n, (unsigned long long)e[j].v);
		}
	pc_pass();
	}
#endif
}
