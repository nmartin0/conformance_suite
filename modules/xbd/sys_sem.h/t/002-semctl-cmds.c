/*
 * GETNCNT, GETPID, GETVAL, GETALL, GETZCNT, SETVAL, SETALL, IPC_RMID,
 * IPC_SET and IPC_STAT have ten different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_sem.h.002
 */
#include "pc.h"
#include <sys/sem.h>

int main(void)
{
	pc_begin("XBD.sys_sem.h.002");
#if !defined(GETNCNT) || !defined(GETPID) || !defined(GETVAL) || !defined(GETALL) || !defined(GETZCNT) || !defined(SETVAL) || !defined(SETALL) || !defined(IPC_RMID) || !defined(IPC_SET) || !defined(IPC_STAT)
	pc_unresolved("a constant is missing (the header check reports it): GETNCNT GETPID GETVAL GETALL GETZCNT SETVAL SETALL IPC_RMID IPC_SET IPC_STAT");
#else
	{
	struct { const char *n; long long v; } e[11];
	int n = 0, i, j;

	e[n].n = "GETNCNT"; e[n++].v = (long long)(GETNCNT);
	e[n].n = "GETPID"; e[n++].v = (long long)(GETPID);
	e[n].n = "GETVAL"; e[n++].v = (long long)(GETVAL);
	e[n].n = "GETALL"; e[n++].v = (long long)(GETALL);
	e[n].n = "GETZCNT"; e[n++].v = (long long)(GETZCNT);
	e[n].n = "SETVAL"; e[n++].v = (long long)(SETVAL);
	e[n].n = "SETALL"; e[n++].v = (long long)(SETALL);
	e[n].n = "IPC_RMID"; e[n++].v = (long long)(IPC_RMID);
	e[n].n = "IPC_SET"; e[n++].v = (long long)(IPC_SET);
	e[n].n = "IPC_STAT"; e[n++].v = (long long)(IPC_STAT);
	if (n < 10)
		pc_unresolved("fewer than 10 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
