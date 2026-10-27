/*
 * P_PGID selects a process group.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.waitid.012
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
	pid_t c1, c2;
	int r;

	pc_begin("XSH.waitid.012");
	guard(20);
	c1 = fork();
	if (c1 == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c1 == 0) {
		(void)setpgid(0, 0);	/* a group of its own */
		sleep_ms(500);
		_exit(2);
	}
	(void)setpgid(c1, c1);		/* both sides, to close the race */
	if (getpgid(c1) != c1) {
		bury(c1);
		pc_unresolved("the child could not be given a process group of its own");
	}
	c2 = kid(0, 1);			/* stays in the caller's group and ends at once */
	pc_test();
	dirty(&si);
	r = waitid(P_PGID, (id_t)c1, &si, WEXITED);
	pc_check(r == 0, "waitid(P_PGID) returned %d (%s), expected 0", r, pc_errname(errno));
	pc_check(si.si_pid == c1, "waitid(P_PGID, %ld) reported child %ld", (long)c1, (long)si.si_pid);
	check_info(&si, c1, CLD_EXITED, 2);
	dirty(&si);
	r = waitid(P_PID, (id_t)c2, &si, WEXITED);
	pc_check(r == 0, "waitid(P_PID) of the other child returned %d (%s): its status should still be there", r, pc_errname(errno));
	check_info(&si, c2, CLD_EXITED, 1);
	pc_pass();
}
