/*
 * sigfillset.c - conforming variants of sigfillset(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <signal.h>
#include "pcvariant.h"

/*
 * "omits_extra": sigfillset() leaves out the implementation's own named
 * signals that POSIX.1-2024 does not define (SIGSTKFLT, SIGPWR, SIGINFO,
 * SIGEMT, SIGLOST, and SIGPROF and SIGPOLL, which Issue 8 removed, where
 * the C library names them and they are not one of the POSIX signals). XSH sigfillset() requires only the signals the
 * standard defines to be included, and the RATIONALE of XSH sigemptyset()
 * says an implementation-defined signal may be better left out of a
 * filled set.
 */
static const int extra[] = {
#ifdef SIGSTKFLT
	SIGSTKFLT,
#endif
#ifdef SIGPWR
	SIGPWR,
#endif
#ifdef SIGINFO
	SIGINFO,
#endif
#ifdef SIGEMT
	SIGEMT,
#endif
#ifdef SIGLOST
	SIGLOST,
#endif
#ifdef SIGPROF
	SIGPROF,
#endif
#ifdef SIGPOLL
	SIGPOLL,
#endif
	0
};

/* The POSIX signals (XBD <signal.h>); an "extra" equal to one is kept. */
static int is_posix(int s)
{
	static const int p[] = {
		SIGABRT, SIGALRM, SIGBUS, SIGCHLD, SIGCONT, SIGFPE, SIGHUP, SIGILL,
		SIGINT, SIGKILL, SIGPIPE, SIGQUIT, SIGSEGV, SIGSTOP, SIGTERM, SIGTSTP,
		SIGTTIN, SIGTTOU, SIGUSR1, SIGUSR2, SIGURG, SIGWINCH, SIGSYS, SIGTRAP,
		SIGVTALRM, SIGXCPU, SIGXFSZ,
	};
	unsigned i;

	if (s >= SIGRTMIN && s <= SIGRTMAX)
		return 1;
	for (i = 0; i < sizeof p / sizeof p[0]; i++)
		if (p[i] == s)
			return 1;
	return 0;
}

int sigfillset(sigset_t *set)
{
	const char *v = variant_of("sigfillset.");
	int r, i;
	REAL(sigfillset);

	r = real_sigfillset(set);
	if (r == 0 && IS("omits_extra"))
		for (i = 0; extra[i] != 0; i++)
			if (!is_posix(extra[i]))
				(void)sigdelset(set, extra[i]);
	return r;
}
