/*
 * WSTOPPED reports a stopped child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.waitid.013
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

static void check_info(const siginfo_t *si, pid_t c, int code, int status)
{
	pc_check(si->si_signo == SIGCHLD, "si_signo is %d, expected SIGCHLD (%d)", si->si_signo, SIGCHLD);
	pc_check(si->si_pid == c, "si_pid is %ld, expected %ld", (long)si->si_pid, (long)c);
	pc_check(si->si_code == code, "si_code is %d, expected %d", si->si_code, code);
	pc_check(si->si_status == status, "si_status is %d, expected %d", si->si_status, status);
}

int main(void)
{
	siginfo_t si;
	pid_t c;
	int r, e;

	pc_begin("XSH.waitid.013");
	guard(20);
	c = stopper();
	pc_test();
	dirty(&si);
	r = waitid(P_PID, (id_t)c, &si, WSTOPPED);
	e = errno;
	bury(c);
	pc_check(r == 0, "waitid(WSTOPPED) returned %d (%s), expected 0", r, pc_errname(e));
	check_info(&si, c, CLD_STOPPED, SIGSTOP);
	pc_pass();
}
