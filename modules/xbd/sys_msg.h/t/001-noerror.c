/*
 * MSG_NOERROR is nonzero and shares no bits with IPC_NOWAIT, the other
 * flag of msgrcv().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_msg.h.001
 */
#include "pc.h"
#include <sys/msg.h>

int main(void)
{
	pc_begin("XBD.sys_msg.h.001");
#if !defined(MSG_NOERROR) || !defined(IPC_NOWAIT)
	pc_unresolved("a constant is missing (the header check reports it): MSG_NOERROR IPC_NOWAIT");
#else
	{
	struct { const char *n; long long v; } e[3];
	int n = 0, i, j;

	e[n].n = "MSG_NOERROR"; e[n++].v = (long long)(MSG_NOERROR);
	e[n].n = "IPC_NOWAIT"; e[n++].v = (long long)(IPC_NOWAIT);
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
