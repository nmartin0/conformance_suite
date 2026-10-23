/*
 * wait() clears a blocked, pending SIGCHLD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.019
 * MUTANT    wait.sigchld_left
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
	sigset_t s;
	pid_t c;
	int st;

	pc_begin("XSH.wait.019");
	{
		/*
		 * Caught, not SIG_DFL: a blocked signal whose action is to
		 * ignore may be discarded at generation (XSH 2.4.1). Installed
		 * before the fork (changing it after is unspecified, 2.4.3).
		 */
		struct sigaction sa;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = on_alarm;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGCHLD, &sa, NULL) == 0, "sigaction failed");
	}
	sigemptyset(&s);
	sigaddset(&s, SIGCHLD);
	pc_check(sigprocmask(SIG_BLOCK, &s, NULL) == 0, "sigprocmask failed");
	c = child(0, 0);
	{
		struct timespec ts = { 0, 10000000 };
		int i;

		for (i = 0; i < 500; i++) {
			pc_check(sigpending(&s) == 0, "sigpending failed");
			if (sigismember(&s, SIGCHLD) == 1)
				break;
			(void)nanosleep(&ts, NULL);
		}
	}
	pc_check(sigismember(&s, SIGCHLD) == 1, "SIGCHLD is not pending within 5 s");
	pc_test();
	pc_check(wait(&st) == c, "wait() failed");
	pc_check(sigpending(&s) == 0, "sigpending failed");
	pc_check(sigismember(&s, SIGCHLD) != 1, "SIGCHLD is still pending after wait() reported the only child");
	pc_pass();
}
