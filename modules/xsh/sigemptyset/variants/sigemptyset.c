/*
 * sigemptyset.c - conforming variants of sigemptyset(), for the variant
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
 * "keeps_extra": sigemptyset() leaves the implementation's own named
 * signals that POSIX.1-2024 does not define (SIGSTKFLT, SIGPWR, SIGINFO,
 * SIGEMT, SIGLOST, and SIGPROF and SIGPOLL, which Issue 8 removed, where
 * the C library names them and they are not one of the POSIX signals) in
 * the set. XSH sigemptyset() requires only the signals the standard
 * defines to be excluded; its RATIONALE puts no requirement on signals
 * that are extensions (it only recommends that they be affected).
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

int sigemptyset(sigset_t *set)
{
	const char *v = variant_of("sigemptyset.");
	int r, i;
	REAL(sigemptyset);

	r = real_sigemptyset(set);
	if (r == 0 && IS("keeps_extra"))
		for (i = 0; extra[i] != 0; i++)
			if (!is_posix(extra[i]))
				(void)sigaddset(set, extra[i]);
	return r;
}
