/*
 * SIGCHLD's siginfo_t names the child, its user and status.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.4.3-signal-actions.019
 * VARIANT   sigaction.late_delivery
 * VARIANT   sigaction.siginfo_rt_only
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero. */
static int wait_for(volatile sig_atomic_t *flag, long ms)
{
	long i;

	for (i = 0; i < ms / 10 && !*flag; i++)
		sleep_ms(10);
	return *flag != 0;
}

static long ms_since(const struct timespec *t0)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long)(t.tv_sec - t0->tv_sec) * 1000 + (t.tv_nsec - t0->tv_nsec) / 1000000;
}

static volatile sig_atomic_t got;
static volatile int code, status;
static volatile pid_t pid;
static volatile uid_t uid;

static void hc(int s, siginfo_t *i, void *u)
{
	(void)s;
	(void)u;
	code = i->si_code;
	pid = i->si_pid;
	uid = i->si_uid;
	status = i->si_status;
	got = 1;
}

int main(void)
{
	pc_begin("XSH.2.4.3-signal-actions.019");
	{
		struct sigaction sa;
		pid_t kid;
		int st;

		memset(&sa, 0, sizeof sa);
		sa.sa_sigaction = hc;
		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		if (sigaction(SIGCHLD, &sa, NULL) != 0) {
			int e = errno;

			/* XSH sigaction() ERRORS: only a system without XSI may refuse SA_SIGINFO outside SIGRTMIN..SIGRTMAX */
			if (e == EINVAL && sysconf(_SC_XOPEN_UNIX) == -1)
				pc_unresolved("sigaction() refused SA_SIGINFO for SIGCHLD (EINVAL) on a system without XSI, which it may do");
			pc_test();
			pc_fail("sigaction() with SA_SIGINFO for SIGCHLD failed: %s", pc_errname(e));
		}
		pc_test();
		kid = fork();
		if (kid == -1)
			pc_unresolved("fork failed: %s", pc_errname(errno));
		if (kid == 0)
			_exit(7);
		pc_check(wait_for(&got, 3000), "the SIGCHLD handler did not run");
		while (waitpid(kid, &st, 0) == -1 && errno == EINTR)
			;
		pc_check(code == CLD_EXITED, "si_code is %d, expected CLD_EXITED (%d)", code, CLD_EXITED);
		pc_check(pid == kid, "si_pid is %ld, expected %ld", (long)pid, (long)kid);
		pc_check(uid == getuid(), "si_uid is %ld, expected %ld", (long)uid, (long)getuid());
		pc_check(status == 7, "si_status is %d, expected 7", status);
	}
	pc_pass();
}
