/*
 * The child's process times start at zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.007
 * MUTANT    fork.times_inherited
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/times.h>
#include <time.h>

/* Uses about ms milliseconds of CPU. */
static void burn(long ms)
{
	struct timespec a, b;
	volatile unsigned long x = 0;

	clock_gettime(CLOCK_MONOTONIC, &a);
	do {
		for (int i = 0; i < 100000; i++)
			x += (unsigned long)i;
		clock_gettime(CLOCK_MONOTONIC, &b);
	} while ((b.tv_sec - a.tv_sec) * 1000L + (b.tv_nsec - a.tv_nsec) / 1000000L < ms);
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
	struct tms t;
	pid_t r;
	int code;

	pc_begin("XSH.fork.007");
	burn(300);
	r = fork();
	pc_check(r != -1, "fork failed");
	if (r == 0) {
		burn(300);
		_exit(0);
	}
	(void)reap(r);
	pc_check(times(&t) != (clock_t)-1, "times failed");
	pc_check(t.tms_utime + t.tms_stime > 5 && t.tms_cutime + t.tms_cstime > 5,
	    "the parent did not accumulate CPU time");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		struct tms c;
		if (times(&c) == (clock_t)-1)
			_exit(9);
		if (c.tms_cutime != 0 || c.tms_cstime != 0)
			_exit(1);
		if (c.tms_utime + c.tms_stime >= t.tms_utime + t.tms_stime)
			_exit(2);
		_exit(0);
	}
	code = reap(r);
	pc_check(code != 1, "the child inherited tms_cutime/tms_cstime");
	pc_check(code != 2, "the child inherited tms_utime/tms_stime");
	pc_check(code == 0, "child status %d", code);
	pc_pass();
}
