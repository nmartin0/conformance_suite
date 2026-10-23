/*
 * wait() waits for a child and reports its exit status.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.001
 * MUTANT    wait.status_garbage
 * MUTANT    wait.wrong_pid
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
	int st = -1;
	long t0;

	pc_begin("XSH.wait.001");
	guard(10);
	pc_test();
	t0 = now_ms();
	c = child(500, 42);
	r = wait(&st);
	pc_check(r == c, "wait() returned %ld, the child is %ld", (long)r, (long)c);
	pc_check(now_ms() - t0 >= 400, "wait() returned before the child exited");
	pc_check(WIFEXITED(st), "WIFEXITED is false for a child that called _exit()");
	pc_check(!WIFSIGNALED(st), "WIFSIGNALED is also true for an exited child");
	pc_check(WEXITSTATUS(st) == 42, "WEXITSTATUS is %d, expected 42", WEXITSTATUS(st));
	pc_pass();
}
