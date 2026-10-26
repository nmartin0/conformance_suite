/*
 * sysconf(_SC_NSIG) is one more than the highest supported signal number.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sysconf.015
 * MUTANT    sysconf.nsig_off_by_one
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
#if !defined(_SC_NSIG) || !defined(NSIG_MAX)
	pc_begin("XSH.sysconf.015");
	pc_unresolved("<unistd.h> lacks _SC_NSIG or <limits.h> lacks NSIG_MAX");
#else
	static const int sigs[] = { SIGABRT, SIGALRM, SIGBUS, SIGCHLD, SIGCONT, SIGFPE, SIGHUP, SIGILL, SIGINT,
		SIGKILL, SIGPIPE, SIGQUIT, SIGSEGV, SIGSTOP, SIGTERM, SIGTSTP, SIGTTIN, SIGTTOU, SIGUSR1, SIGUSR2,
		SIGURG };
	struct sigaction oa;
	size_t i;
	long n;
	int r, e;

	pc_begin("XSH.sysconf.015");
	pc_test();
	n = sysconf(_SC_NSIG);
	pc_check(n > SIGRTMAX, "sysconf(_SC_NSIG) returned %ld, not above SIGRTMAX (%d)", n, SIGRTMAX);
	for (i = 0; i < sizeof sigs / sizeof sigs[0]; i++)
		pc_check(n > sigs[i], "sysconf(_SC_NSIG) returned %ld, not above signal %d", n, sigs[i]);
	pc_check(n <= NSIG_MAX, "sysconf(_SC_NSIG) returned %ld, above NSIG_MAX (%ld)", n, (long)NSIG_MAX);
	if (n > INT_MAX)
		pc_unresolved("sysconf(_SC_NSIG) does not fit in an int");
	errno = 0;
	r = sigaction((int)n, NULL, &oa);
	e = errno;
	pc_check(r == -1 && e == EINVAL, "sigaction(%ld, NULL, &old) returned %d (errno %s): %ld is a supported signal",
		 n, r, pc_errname(e), n);
	pc_pass();
#endif
}
