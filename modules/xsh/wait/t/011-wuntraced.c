/*
 * WUNTRACED reports a stopped child.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.011
 * MUTANT    wait.untraced_ignored
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

/* A child that stops itself with SIGSTOP, then exits 0 once continued. */
static pid_t stopper(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0) {
		(void)raise(SIGSTOP);
		_exit(0);
	}
	return r;
}

int main(void)
{
	pid_t c;
	int st;

	pc_begin("XSH.wait.011");
	guard(10);
	pc_test();
	c = stopper();
	pc_check(waitpid(c, &st, WUNTRACED) == c, "waitpid(WUNTRACED) did not report the stopped child (%s)",
	    pc_errname(errno));
	pc_check(WIFSTOPPED(st), "WIFSTOPPED is false");
	pc_check(WSTOPSIG(st) == SIGSTOP, "WSTOPSIG is %d, expected SIGSTOP (%d)", WSTOPSIG(st), SIGSTOP);
	pc_check(!WIFEXITED(st) && !WIFSIGNALED(st), "another status macro is also true");
	(void)kill(c, SIGCONT);
	(void)waitpid(c, &st, 0);
	pc_pass();
}
