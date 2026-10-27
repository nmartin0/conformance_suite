/*
 * IPC_RMID, IPC_SET and IPC_STAT have three different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_ipc.h.002
 */
#include "pc.h"
#include <sys/ipc.h>

int main(void)
{
	pc_begin("XBD.sys_ipc.h.002");
#if !defined(IPC_RMID) || !defined(IPC_SET) || !defined(IPC_STAT)
	pc_unresolved("a constant is missing (the header check reports it): IPC_RMID IPC_SET IPC_STAT");
#else
	{
	struct { const char *n; long long v; } e[4];
	int n = 0, i, j;

	e[n].n = "IPC_RMID"; e[n++].v = (long long)(IPC_RMID);
	e[n].n = "IPC_SET"; e[n++].v = (long long)(IPC_SET);
	e[n].n = "IPC_STAT"; e[n++].v = (long long)(IPC_STAT);
	if (n < 3)
		pc_unresolved("fewer than 3 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
