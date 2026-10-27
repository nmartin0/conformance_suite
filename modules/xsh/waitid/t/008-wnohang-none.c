/*
 * WNOHANG with no status available.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.waitid.008
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

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* A child that lives ms milliseconds and then calls _exit(code). */
static pid_t kid(long ms, int code)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (r == 0) {
		sleep_ms(ms);
		_exit(code);
	}
	return r;
}

/* Kills and reaps a child. */
static void bury(pid_t c)
{
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
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
	int r, e;

	pc_begin("XSH.waitid.008");
	guard(20);
	c = kid(3000, 0);
	pc_test();
	dirty(&si);
	r = waitid(P_PID, (id_t)c, &si, WEXITED | WNOHANG);
	e = errno;
	bury(c);
	pc_check(r == 0, "waitid(WNOHANG) of a running child returned %d (%s), expected 0", r, pc_errname(e));
	pc_check(si.si_signo == 0, "si_signo is %d, expected 0", si.si_signo);
	pc_check(si.si_pid == 0, "si_pid is %ld, expected 0", (long)si.si_pid);
	pc_pass();
}
