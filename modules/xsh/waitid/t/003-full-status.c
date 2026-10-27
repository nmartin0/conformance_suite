/*
 * si_status holds the full exit value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.waitid.003
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

int main(void)
{
	siginfo_t si;
	pid_t c;
	int r;

	pc_begin("XSH.waitid.003");
	guard(20);
	c = kid(0, 263);
	pc_test();
	dirty(&si);
	r = waitid(P_PID, (id_t)c, &si, WEXITED);
	pc_check(r == 0, "waitid() returned %d (%s), expected 0", r, pc_errname(errno));
	pc_check(si.si_pid == c, "si_pid is %ld, expected %ld", (long)si.si_pid, (long)c);
	pc_check(si.si_code == CLD_EXITED, "si_code is %d, expected CLD_EXITED", si.si_code);
	pc_check(si.si_status == 263, "si_status is %d for _exit(263), expected 263", si.si_status);
	pc_pass();
}
