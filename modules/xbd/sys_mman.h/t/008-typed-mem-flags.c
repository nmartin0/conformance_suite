/*
 * POSIX_TYPED_MEM_ALLOCATE, POSIX_TYPED_MEM_ALLOCATE_CONTIG and
 * POSIX_TYPED_MEM_MAP_ALLOCATABLE are nonzero and have three different
 * values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.008
 */
#include "pc.h"
#include <sys/mman.h>

int main(void)
{
	pc_begin("XBD.sys_mman.h.008");
#if !defined(POSIX_TYPED_MEM_ALLOCATE) || !defined(POSIX_TYPED_MEM_ALLOCATE_CONTIG) || !defined(POSIX_TYPED_MEM_MAP_ALLOCATABLE)
	pc_unresolved("a constant is missing (the header check reports it): POSIX_TYPED_MEM_ALLOCATE POSIX_TYPED_MEM_ALLOCATE_CONTIG POSIX_TYPED_MEM_MAP_ALLOCATABLE");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "POSIX_TYPED_MEM_ALLOCATE"; e[n++].v = (long long)(POSIX_TYPED_MEM_ALLOCATE);
	e[n].n = "POSIX_TYPED_MEM_ALLOCATE_CONTIG"; e[n++].v = (long long)(POSIX_TYPED_MEM_ALLOCATE_CONTIG);
	e[n].n = "POSIX_TYPED_MEM_MAP_ALLOCATABLE"; e[n++].v = (long long)(POSIX_TYPED_MEM_MAP_ALLOCATABLE);
	if (n < 3)
		pc_unresolved("fewer than 3 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check(e[i].v != 0, "%s is zero", e[i].n);
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
