/*
 * waitpid(0) waits for the caller's process group.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.008
 * MUTANT    wait.zero_as_any
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

/*
 * Two children: a stays in our process group and exits after a_ms;
 * b moves to a group of its own and exits after b_ms. The pipe makes
 * sure b has moved before we go on.
 */
static void two(long a_ms, long b_ms, pid_t *a, pid_t *b)
{
	int p[2];
	char c;

	if (pipe(p) != 0)
		pc_unresolved("pipe failed");
	*b = fork();
	if (*b == -1)
		pc_unresolved("fork failed");
	if (*b == 0) {
		struct timespec ts = { b_ms / 1000, (b_ms % 1000) * 1000000L };
		(void)close(p[0]);
		if (setpgid(0, 0) != 0)
			_exit(9);
		(void)write(p[1], "g", 1);
		(void)close(p[1]);
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
		_exit(2);
	}
	(void)close(p[1]);	/* so that a dead b gives end of file */
	(void)setpgid(*b, *b);	/* also from the parent, to close the race */
	if (read(p[0], &c, 1) != 1)
		pc_unresolved("child b did not start");
	(void)close(p[0]);
	*a = child(a_ms, 1);
}

int main(void)
{
	pid_t a, b, r;
	int st;

	pc_begin("XSH.wait.008");
	guard(10);
	pc_test();
	two(400, 0, &a, &b);
	r = waitpid(0, &st, 0);
	pc_check(r == a, "waitpid(0) returned %ld (a in our group is %ld, b in its own is %ld)",
	    (long)r, (long)a, (long)b);
	pc_check(waitpid(b, &st, 0) == b, "child b was not still waitable");
	pc_pass();
}
