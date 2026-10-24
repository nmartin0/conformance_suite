/*
 * abort.c - conforming variants of abort(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcvariant.h"

/*
 * "flush_streams": an abort() of its own. SIGABRT goes to the calling
 * thread as raise() would send it, with any block on it lifted first; if
 * a handler returns (or there is none), the disposition goes back to
 * SIG_DFL, every open stream is flushed, and the signal is sent again.
 * XSH abort() allows both extras: the abnormal termination may include an
 * attempt to do what fclose() does to all open streams (CX), and when the
 * first SIGABRT does not end the process, abort() may reset SIGABRT to
 * SIG_DFL and send it again. The flush is only part of the termination,
 * so a handler that does not return finds the streams as they were.
 */
_Noreturn void abort(void)
{
	const char *v = variant_of("abort.");
	REAL(abort);

	if (IS("flush_streams")) {
		struct sigaction cur, dfl;
		sigset_t s;

		sigemptyset(&s);
		sigaddset(&s, SIGABRT);
		if (sigaction(SIGABRT, NULL, &cur) == 0 &&
		    ((cur.sa_flags & SA_SIGINFO) ||
		    (cur.sa_handler != SIG_DFL && cur.sa_handler != SIG_IGN))) {
			(void)pthread_sigmask(SIG_UNBLOCK, &s, NULL);
			(void)raise(SIGABRT);	/* the handler runs here */
		}
		memset(&dfl, 0, sizeof dfl);
		dfl.sa_handler = SIG_DFL;
		sigemptyset(&dfl.sa_mask);
		(void)sigaction(SIGABRT, &dfl, NULL);
		(void)fflush(NULL);
	}
	real_abort();
	for (;;)
		;
}
