/*
 * sigwait.c - conforming variants of sigwait(), for the variant checks
 * (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "pcvariant.h"

/* Signals whose default action is to ignore them (XBD <signal.h> table). */
static int default_ignored(int sig)
{
	return sig == SIGCHLD || sig == SIGURG || sig == SIGCONT
#ifdef SIGWINCH
	    || sig == SIGWINCH
#endif
	    ;
}

int sigwait(const sigset_t *set, int *sig)
{
	const char *v = variant_of("sigwait.");
	int r, s;
	REAL(sigwait);

	r = real_sigwait(set, sig);
	if (r != 0)
		return r;
	/*
	 * "action_dfl": after taking a signal, sigwait() sets the action of
	 * every signal in set to SIG_DFL. XSH sigwait() DESCRIPTION leaves
	 * the effect of sigwait() on the actions of the signals in set
	 * unspecified. Signals whose default is to ignore them keep their
	 * action: setting it would discard their pending instances (XSH
	 * sigaction()), which the rest of sigwait() does not allow (queued
	 * instances must remain).
	 */
	if (IS("action_dfl")) {
		struct sigaction sa;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = SIG_DFL;
		sigemptyset(&sa.sa_mask);
		for (s = 1; s < 65; s++)
			if (sigismember(set, s) == 1 && s != SIGKILL && s != SIGSTOP && !default_ignored(s))
				(void)sigaction(s, &sa, NULL);
	}
	/*
	 * "late": sigwait() returns 700 ms after it took the signal, as if the
	 * thread were not scheduled for that long. Nothing in XSH sigwait()
	 * or XSH 2.4 bounds how soon a waiting thread resumes once a signal
	 * in its set is pending; the signal has already been cleared, so no
	 * other waiter can take it meanwhile.
	 */
	if (IS("late")) {
		struct timespec ts = { 0, 700000000L };

		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
	}
	return 0;
}
