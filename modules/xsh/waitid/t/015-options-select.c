/*
 * Only the named state changes are reported.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.waitid.015
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* A call that blocks must fail the test, not hang it. */
static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: the call under test did not finish in time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

/* Kills and reaps a child. */
static void bury(pid_t c)
{
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
}

/* A child that stops itself with SIGSTOP and, once continued, waits to be killed. */
static pid_t stopper(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (r == 0) {
		(void)raise(SIGSTOP);
		for (;;)
			(void)pause();
	}
	return r;
}

/* Fills the structure with bytes that no member should keep. */
static void dirty(siginfo_t *si)
{
	memset(si, 0xff, sizeof *si);
}

int main(void)
{
	siginfo_t si;
	pid_t c;
	int r;

	pc_begin("XSH.waitid.015");
	guard(20);
	c = stopper();
	/* Setup: wait until the child has stopped, leaving the stop in place. */
	r = waitid(P_PID, (id_t)c, &si, WSTOPPED | WNOWAIT);
	if (r != 0) {
		bury(c);
		pc_unresolved("the child's stop could not be observed (%s)", pc_errname(errno));
	}
	pc_test();
	dirty(&si);
	r = waitid(P_PID, (id_t)c, &si, WEXITED | WNOHANG);
	if (r != 0 || si.si_pid != 0 || si.si_signo != 0) {
		int e = errno;

		bury(c);
		pc_fail("waitid(WEXITED|WNOHANG) of a stopped child gave %d (%s), si_pid %ld, si_signo %d: a stop does not match WEXITED",
		    r, pc_errname(e), (long)si.si_pid, si.si_signo);
	}
	(void)kill(c, SIGKILL);
	/* The child has now been killed: wait for that without taking the status. */
	r = waitid(P_PID, (id_t)c, &si, WEXITED | WNOWAIT);
	pc_check(r == 0, "waitid(WEXITED|WNOWAIT) of the killed child returned %d (%s)", r, pc_errname(errno));
	dirty(&si);
	r = waitid(P_PID, (id_t)c, &si, WSTOPPED | WNOHANG);
	pc_check(r == 0, "waitid(WSTOPPED|WNOHANG) of a terminated child returned %d (%s), expected 0", r, pc_errname(errno));
	pc_check(si.si_pid == 0 && si.si_signo == 0, "a terminated child was reported to WSTOPPED (si_pid %ld, si_code %d)", (long)si.si_pid, si.si_code);
	(void)waitpid(c, NULL, 0);
	pc_pass();
}
