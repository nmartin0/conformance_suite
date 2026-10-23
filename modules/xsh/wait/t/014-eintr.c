/*
 * An interrupted wait() gives EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.014
 * MUTANT    wait.eintr_restarts
 * MUTANT    wait.eintr_as_echild
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

	pc_begin("XSH.wait.014");
	c = child(3000, 0);
	guard(1);
	pc_test();
	r = wait(&st);
	pc_check(r == -1, "wait() returned %ld instead of being interrupted", (long)r);
	pc_check(errno == EINTR, "errno %s, expected EINTR", pc_errname(errno));
	(void)waitpid(c, &st, 0);
	pc_pass();
}
