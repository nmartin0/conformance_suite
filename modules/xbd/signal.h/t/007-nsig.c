/*
 * Signal numbers are below sysconf(_SC_NSIG) and {NSIG_MAX}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.007
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

#include <limits.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XBD.signal.h.007");
	pc_test();
#if !defined(_SC_NSIG)
	pc_fail("<unistd.h> does not define _SC_NSIG");
#else
	{
		static const int named[] = { SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM, SIGALRM, SIGHUP, SIGKILL, SIGPIPE, SIGQUIT, SIGUSR1, SIGUSR2, SIGCHLD, SIGCONT, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGBUS, SIGURG, SIGWINCH };
		long ns = sysconf(_SC_NSIG), lim;
		size_t i;

		pc_check(ns > 0, "sysconf(_SC_NSIG) gave %ld", ns);
		pc_check(SIGRTMAX < ns, "SIGRTMAX (%d) is not below sysconf(_SC_NSIG) (%ld)", SIGRTMAX, ns);
#ifdef NSIG_MAX
		lim = NSIG_MAX;
#else
		lim = ns;
#endif
		for (i = 0; i < sizeof named / sizeof named[0]; i++)
			pc_check(named[i] < lim, "signal %d is not below {NSIG_MAX} (%ld)", named[i], lim);
	}
	pc_pass();
#endif
}
