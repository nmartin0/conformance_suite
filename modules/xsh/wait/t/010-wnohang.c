/*
 * WNOHANG returns 0 for a running child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.010
 * MUTANT    wait.nohang_blocks
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t fired;

static void on_alarm(int sig)
{
	(void)sig;
	fired = 1;
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

/* A child that does nothing but sleep for ms milliseconds, then _exit(code). */
static pid_t child(long ms, int code)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		struct timespec ts;
		ts.tv_sec = ms / 1000;
		ts.tv_nsec = (ms % 1000) * 1000000L;
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
		_exit(code);
	}
	return r;
}

static long now_ms(void)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}

int main(void)
{
	pid_t c, r;
	int st;

	pc_begin("XSH.wait.010");
	guard(5);
	pc_test();
	c = child(2000, 0);
	r = waitpid(c, &st, WNOHANG);
	pc_check(r == 0, "waitpid(WNOHANG) on a running child returned %ld (%s)",
	    (long)r, r == -1 ? pc_errname(errno) : "-");
	(void)kill(c, SIGKILL);
	(void)waitpid(c, &st, 0);
	pc_pass();
}
