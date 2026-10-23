/*
 * CPT: the child's CPU-time clock starts at zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.015
 * MUTANT    fork.cputime_inherited
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

/* Uses ms milliseconds of process CPU time (wall-clock cap 10 s). */
static void burn(long ms)
{
	struct timespec c, w0, w;
	volatile unsigned long x = 0;

	clock_gettime(CLOCK_MONOTONIC, &w0);
	do {
		for (int i = 0; i < 100000; i++)
			x += (unsigned long)i;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &c);
		clock_gettime(CLOCK_MONOTONIC, &w);
	} while (c.tv_sec * 1000L + c.tv_nsec / 1000000L < ms && w.tv_sec - w0.tv_sec < 10);
}

/*
 * Waits for the child; returns its exit status (0 = all its checks held).
 * A child that did not exit normally is UNRESOLVED.
 */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (!WIFEXITED(st))
		pc_unresolved("the child was killed by signal %d", WIFSIGNALED(st) ? WTERMSIG(st) : 0);
	return WEXITSTATUS(st);
}

int main(void)
{
	struct timespec p;
	pid_t r;
	int code;

	pc_begin("XSH.fork.015");
	burn(300);
	pc_check(clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &p) == 0, "clock_gettime failed");
	pc_check(p.tv_sec > 0 || p.tv_nsec > 200000000, "the parent did not use CPU time");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		struct timespec c;
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &c) != 0)
			_exit(9);
		_exit((long long)c.tv_sec * 1000000000LL + c.tv_nsec <
		    ((long long)p.tv_sec * 1000000000LL + p.tv_nsec) / 2 ? 0 : 1);
	}
	code = reap(r);
	pc_check(code == 0, "the child's CPU-time clock did not start at zero");
	pc_pass();
}
