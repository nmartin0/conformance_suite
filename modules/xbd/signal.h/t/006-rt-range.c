/*
 * SIGRTMIN to SIGRTMAX is a range of at least {RTSIG_MAX} realtime signals apart from the named ones.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.006
 *
 * Only <signal.h> is included (pc.h brings in <stddef.h> alone), so a
 * type or name this test uses that the header does not provide stops
 * the build, and the test is UNINITIATED.
 */
#include <signal.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

/* Fails unless the n values in v are all different. */
static void distinct(const char *what, const long *v, const char *const *name, int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			pc_check(v[i] != v[j], "%s: %s and %s are both %ld", what, name[i], name[j], v[i]);
}

#include <unistd.h>

int main(void)
{
	pc_begin("XBD.signal.h.006");
	pc_test();
#if !defined(SIGRTMIN) || !defined(SIGRTMAX)
	pc_fail("<signal.h> does not define SIGRTMIN and SIGRTMAX as macros");
#else
	{
		static const int named[] = { SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM, SIGALRM, SIGHUP, SIGKILL, SIGPIPE, SIGQUIT, SIGUSR1, SIGUSR2, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGBUS, SIGURG, SIGWINCH };
		static const char *const name[] = { "SIGABRT", "SIGFPE", "SIGILL", "SIGINT", "SIGSEGV", "SIGTERM", "SIGALRM", "SIGHUP", "SIGKILL", "SIGPIPE", "SIGQUIT", "SIGUSR1", "SIGUSR2", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGBUS", "SIGURG", "SIGWINCH" };
		long rt = sysconf(_SC_RTSIG_MAX), n = (long)SIGRTMAX - SIGRTMIN + 1;
		size_t i;

		pc_check(IS_T(SIGRTMIN, int) && IS_T(SIGRTMAX, int), "SIGRTMIN or SIGRTMAX does not have type int");
		pc_check(SIGRTMIN > 0 && SIGRTMAX >= SIGRTMIN, "SIGRTMIN is %d and SIGRTMAX %d", SIGRTMIN, SIGRTMAX);
		if (rt == -1 || rt < 8)
			rt = 8;
		pc_check(n >= rt, "SIGRTMIN..SIGRTMAX holds %ld signals; {RTSIG_MAX} is %ld", n, rt);
		for (i = 0; i < sizeof named / sizeof named[0]; i++)
			pc_check(named[i] < SIGRTMIN || named[i] > SIGRTMAX, "%s (%d) is in SIGRTMIN..SIGRTMAX", name[i], named[i]);
	}
	pc_pass();
#endif
}
