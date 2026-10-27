/*
 * The POSIX_FADV_* advice constants (NORMAL, SEQUENTIAL, RANDOM, WILLNEED,
 * DONTNEED, NOREUSE) have six different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.016
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.016");
#if !defined(POSIX_FADV_NORMAL) || !defined(POSIX_FADV_SEQUENTIAL) || !defined(POSIX_FADV_RANDOM) || !defined(POSIX_FADV_WILLNEED) || !defined(POSIX_FADV_DONTNEED) || !defined(POSIX_FADV_NOREUSE)
	pc_unresolved("a constant is missing (the header check reports it): POSIX_FADV_NORMAL POSIX_FADV_SEQUENTIAL POSIX_FADV_RANDOM POSIX_FADV_WILLNEED POSIX_FADV_DONTNEED POSIX_FADV_NOREUSE");
#else
	{
	struct { const char *n; long long v; } e[7];
	int n = 0, i, j;

	e[n].n = "POSIX_FADV_NORMAL"; e[n++].v = (long long)(POSIX_FADV_NORMAL);
	e[n].n = "POSIX_FADV_SEQUENTIAL"; e[n++].v = (long long)(POSIX_FADV_SEQUENTIAL);
	e[n].n = "POSIX_FADV_RANDOM"; e[n++].v = (long long)(POSIX_FADV_RANDOM);
	e[n].n = "POSIX_FADV_WILLNEED"; e[n++].v = (long long)(POSIX_FADV_WILLNEED);
	e[n].n = "POSIX_FADV_DONTNEED"; e[n++].v = (long long)(POSIX_FADV_DONTNEED);
	e[n].n = "POSIX_FADV_NOREUSE"; e[n++].v = (long long)(POSIX_FADV_NOREUSE);
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
