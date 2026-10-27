/*
 * system() leaves the status of other children alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.016
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

int main(void)
{
	pid_t a, b;
	int r, st;

	pc_begin("XSH.system.016");
	guard(30);
	a = kid(0, 7);		/* ends before the call */
	b = kid(600, 8);	/* ends during the call */
	sleep_ms(300);
	pc_test();
	r = system("sleep 1; exit 3");
	pc_check(r != -1 && WIFEXITED(r) && WEXITSTATUS(r) == 3, "system() gave %d", r);
	pc_check(waitpid(a, &st, 0) == a, "the first child could not be waited for afterwards: %s", pc_errname(errno));
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 7, "first child status %#x, expected exit 7", st);
	pc_check(waitpid(b, &st, 0) == b, "the second child could not be waited for afterwards: %s", pc_errname(errno));
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 8, "second child status %#x, expected exit 8", st);
	pc_pass();
}
