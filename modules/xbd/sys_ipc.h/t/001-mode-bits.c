/*
 * IPC_CREAT, IPC_EXCL and IPC_NOWAIT are nonzero; IPC_CREAT and IPC_EXCL
 * share no bits with each other or with the permission bits 0777.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_ipc.h.001
 */
#include "pc.h"
#include <sys/ipc.h>

int main(void)
{
	pc_begin("XBD.sys_ipc.h.001");
#if !defined(IPC_CREAT) || !defined(IPC_EXCL) || !defined(IPC_NOWAIT)
	pc_unresolved("a constant is missing (the header check reports it): IPC_CREAT IPC_EXCL IPC_NOWAIT");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "IPC_CREAT"; e[n++].v = (long long)(IPC_CREAT);
	e[n].n = "IPC_EXCL"; e[n++].v = (long long)(IPC_EXCL);
	e[n].n = "IPC_NOWAIT"; e[n++].v = (long long)(IPC_NOWAIT);
	if (n < 2)
		pc_unresolved("fewer than 2 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		pc_check(e[i].v != 0, "%s is zero", e[i].n);
	/* Only IPC_CREAT and IPC_EXCL (e[0], e[1]) are OR-ed with the permission bits. */
	for (i = 0; i < 2; i++)
		for (j = i + 1; j < 2; j++) {
			pc_check(((unsigned long long)e[i].v & (unsigned long long)e[j].v) == 0, "%s (%#llx) and %s (%#llx) have bits in common", e[i].n, (unsigned long long)e[i].v, e[j].n, (unsigned long long)e[j].v);
		}
	for (i = 0; i < 2; i++)
		pc_check((e[i].v & 0777) == 0, "%s (%#llo) overlaps the permission bits", e[i].n, (unsigned long long)e[i].v);
	pc_pass();
	}
#endif
}
