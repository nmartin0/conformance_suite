/*
 * killpg.c - conforming variants of killpg(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "late": a signal for a process group the caller is not in reaches the
 * group's members about a second after killpg() returns. killpg() is
 * kill(-pgrp, sig) (XSH killpg() DESCRIPTION), and XSH 2.4.1 sets no bound
 * on how long a generated signal stays pending before delivery. Errors are
 * decided at the call with the null signal; the send is then made by a
 * helper process with the caller's credentials and all signals blocked.
 * As with kill.late, the helper generates the signal late, so tests
 * declare this only where the members catch it unblocked.
 */
#define LATE_MS 1000

int killpg(pid_t pgrp, int sig)
{
	const char *v = variant_of("killpg.");
	sigset_t s, all;
	int e = errno;
	pid_t c, g;
	REAL(killpg);

	sigemptyset(&s);
	if (!IS("late") || sig <= 0 || sigaddset(&s, sig) != 0 || sig == SIGKILL ||
	    sig == SIGSTOP || pgrp <= 1 || pgrp == getpgrp())
		return real_killpg(pgrp, sig);
	if (real_killpg(pgrp, 0) != 0)
		return -1;
	c = fork();
	if (c == -1)
		return real_killpg(pgrp, sig);
	if (c == 0) {
		sigfillset(&all);
		(void)sigprocmask(SIG_BLOCK, &all, NULL);
		g = fork();
		if (g == -1)
			(void)real_killpg(pgrp, sig);
		if (g == 0) {
			struct timespec ts = { LATE_MS / 1000, (LATE_MS % 1000) * 1000000L };
			while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
				;
			(void)real_killpg(pgrp, sig);
		}
		_exit(0);
	}
	while (waitpid(c, NULL, 0) == -1 && errno == EINTR)
		;
	errno = e;
	return 0;
}
