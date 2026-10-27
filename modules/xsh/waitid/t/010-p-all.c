/*
 * P_ALL ignores id.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.waitid.010
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
	int r, i, got1 = 0, got2 = 0;

	pc_begin("XSH.waitid.010");
	guard(20);
	c1 = kid(0, 1);
	c2 = kid(200, 2);
	pc_test();
	for (i = 0; i < 2; i++) {
		dirty(&si);
		r = waitid(P_ALL, (id_t)987654321, &si, WEXITED);
		pc_check(r == 0, "waitid(P_ALL) call %d returned %d (%s), expected 0", i + 1, r, pc_errname(errno));
		if (si.si_pid == c1) {
			pc_check(!got1, "child 1 was reported twice");
			got1 = 1;
			check_info(&si, c1, CLD_EXITED, 1);
		} else if (si.si_pid == c2) {
			pc_check(!got2, "child 2 was reported twice");
			got2 = 1;
			check_info(&si, c2, CLD_EXITED, 2);
		} else {
			pc_fail("waitid(P_ALL) reported si_pid %ld, which is neither child", (long)si.si_pid);
		}
	}
	pc_check(got1 && got2, "not both children were reported");
	pc_pass();
}
