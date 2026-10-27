/*
 * WEXITED, WNOWAIT, WSTOPPED and WNOHANG are nonzero and share no bits, so
 * they can be OR-ed in the options argument of waitid().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_wait.h.002
 */
#include "pc.h"
#include <sys/wait.h>

int main(void)
{
	pc_begin("XBD.sys_wait.h.002");
#if !defined(WEXITED) || !defined(WNOWAIT) || !defined(WSTOPPED) || !defined(WNOHANG)
	pc_unresolved("a constant is missing (the header check reports it): WEXITED WNOWAIT WSTOPPED WNOHANG");
#else
	{
	struct { const char *n; long long v; } e[5];
	int n = 0, i, j;

	e[n].n = "WEXITED"; e[n++].v = (long long)(WEXITED);
	e[n].n = "WNOWAIT"; e[n++].v = (long long)(WNOWAIT);
	e[n].n = "WSTOPPED"; e[n++].v = (long long)(WSTOPPED);
	e[n].n = "WNOHANG"; e[n++].v = (long long)(WNOHANG);
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
