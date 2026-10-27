/*
 * F_RDLCK, F_UNLCK and F_WRLCK are usable in #if, have different values
 * and fit in l_type (a short).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.010
 */
#include "pc.h"
#include <fcntl.h>

#ifdef F_RDLCK
#if !((F_RDLCK) || 1)
#error "F_RDLCK is not usable in #if"
#endif
#endif
#ifdef F_UNLCK
#if !((F_UNLCK) || 1)
#error "F_UNLCK is not usable in #if"
#endif
#endif
#ifdef F_WRLCK
#if !((F_WRLCK) || 1)
#error "F_WRLCK is not usable in #if"
#endif
#endif

int main(void)
{
	pc_begin("XBD.fcntl.h.010");
#if !defined(F_RDLCK) || !defined(F_UNLCK) || !defined(F_WRLCK)
	pc_unresolved("a constant is missing (the header check reports it): F_RDLCK F_UNLCK F_WRLCK");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "F_RDLCK"; e[n++].v = (long long)(F_RDLCK);
	e[n].n = "F_UNLCK"; e[n++].v = (long long)(F_UNLCK);
	e[n].n = "F_WRLCK"; e[n++].v = (long long)(F_WRLCK);
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check((long long)(short)e[i].v == e[i].v, "%s does not fit in a short", e[i].n);
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
