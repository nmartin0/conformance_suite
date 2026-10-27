/*
 * SHM_RDONLY and SHM_RND are nonzero and share no bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_shm.h.001
 */
#include "pc.h"
#include <sys/shm.h>

int main(void)
{
	pc_begin("XBD.sys_shm.h.001");
#if !defined(SHM_RDONLY) || !defined(SHM_RND)
	pc_unresolved("a constant is missing (the header check reports it): SHM_RDONLY SHM_RND");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "SHM_RDONLY"; e[n++].v = (long long)(SHM_RDONLY);
	e[n].n = "SHM_RND"; e[n++].v = (long long)(SHM_RND);
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check(e[i].v != 0, "%s is zero", e[i].n);
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(((unsigned long long)e[i].v & (unsigned long long)e[j].v) == 0, "%s (%#llx) and %s (%#llx) have bits in common", e[i].n, (unsigned long long)e[i].v, e[j].n, (unsigned long long)e[j].v);
		}
	pc_pass();
	}
#endif
}
