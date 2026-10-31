/*
 * kill() with an invalid signal number fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill.007
 * MUTANT    kill.einval_accepted
 */
#include "pc.h"
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Installs fn for sig (FAIL or UNRESOLVED by phase). */
static void catch_sig(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/* A set holding the listed signals (end the list with 0). */
static sigset_t set_of(int first, ...)
{
	sigset_t s;
	va_list ap;
	int sig;

	sigemptyset(&s);
	va_start(ap, first);
	for (sig = first; sig != 0; sig = va_arg(ap, int))
		sigaddset(&s, sig);
	va_end(ap);
	return s;
}

/* A signal number above every valid one: SIGRTMAX is below sysconf(_SC_NSIG) (Issue 8); INT_MAX where that is missing. */
static int big_sig(void)
{
#ifdef _SC_NSIG
	long n = sysconf(_SC_NSIG);

	if (n > 0 && n <= INT_MAX)
		return (int)n;
#endif
	return INT_MAX;
}

int main(void)
{
	pc_begin("XSH.kill.007");
	{
		/* The number must not be a signal here; if it is, sysconf() is at fault (XSH.sysconf.015), not kill(). */
		struct sigaction old;

		if (sigaction(big_sig(), NULL, &old) == 0)
			pc_unresolved("signal %d, from sysconf(_SC_NSIG), is a supported signal here", big_sig());
	}
	pc_test();
	pc_expect_err(kill(getpid(), -1), EINVAL, "kill(self, -1)");
	pc_expect_err(kill(getpid(), big_sig()), EINVAL, "kill(self, <a number above every valid one>)");
	pc_pass();
}
