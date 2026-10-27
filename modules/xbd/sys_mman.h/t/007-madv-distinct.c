/*
 * POSIX_MADV_NORMAL, POSIX_MADV_RANDOM, POSIX_MADV_SEQUENTIAL,
 * POSIX_MADV_WILLNEED and POSIX_MADV_DONTNEED have five different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.007
 */
#include "pc.h"
#include <sys/mman.h>

int main(void)
{
	pc_begin("XBD.sys_mman.h.007");
#if !defined(POSIX_MADV_NORMAL) || !defined(POSIX_MADV_RANDOM) || !defined(POSIX_MADV_SEQUENTIAL) || !defined(POSIX_MADV_WILLNEED) || !defined(POSIX_MADV_DONTNEED)
	pc_unresolved("a constant is missing (the header check reports it): POSIX_MADV_NORMAL POSIX_MADV_RANDOM POSIX_MADV_SEQUENTIAL POSIX_MADV_WILLNEED POSIX_MADV_DONTNEED");
#else
	{
	struct { const char *n; long long v; } e[6];
	int n = 0, i, j;

	e[n].n = "POSIX_MADV_NORMAL"; e[n++].v = (long long)(POSIX_MADV_NORMAL);
	e[n].n = "POSIX_MADV_RANDOM"; e[n++].v = (long long)(POSIX_MADV_RANDOM);
	e[n].n = "POSIX_MADV_SEQUENTIAL"; e[n++].v = (long long)(POSIX_MADV_SEQUENTIAL);
	e[n].n = "POSIX_MADV_WILLNEED"; e[n++].v = (long long)(POSIX_MADV_WILLNEED);
	e[n].n = "POSIX_MADV_DONTNEED"; e[n++].v = (long long)(POSIX_MADV_DONTNEED);
	if (n < 5)
		pc_unresolved("fewer than 5 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
