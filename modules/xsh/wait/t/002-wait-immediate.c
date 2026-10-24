/*
 * wait() returns at once when status is waiting.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.002
 * MUTANT    wait.status_garbage
 * MUTANT    wait.slow_when_ready
 * VARIANT   wait.status_encoding
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
	long t0;

	pc_begin("XSH.wait.002");
	guard(10);
	{
		/*
		 * SIGCHLD caught (so it stays pending while blocked) and
		 * blocked: once it is pending the child has terminated and its
		 * status is available.
		 */
		struct sigaction sa;
		sigset_t set, pend;
		struct timespec ts = { 0, 10000000 };
		int i;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = on_alarm;
		sigemptyset(&sa.sa_mask);
		pc_check(sigaction(SIGCHLD, &sa, NULL) == 0, "sigaction failed");
		sigemptyset(&set);
		sigaddset(&set, SIGCHLD);
		pc_check(sigprocmask(SIG_BLOCK, &set, NULL) == 0, "sigprocmask failed");
		c = child(0, 3);
		{
			siginfo_t si;

			memset(&si, 0, sizeof si);
			pc_check(waitid(P_PID, (id_t)c, &si, WEXITED | WNOWAIT) == 0, "waitid(WNOWAIT) failed: %s", pc_errname(errno));
		}
		(void)pend;
		(void)ts;
		(void)i;
	}
	pc_test();
	t0 = now_ms();
	r = wait(&st);
	pc_check(r == c, "wait() returned %ld", (long)r);
	pc_check(now_ms() - t0 < 1000, "wait() took %ld ms with status already available", now_ms() - t0);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 3, "wrong status");
	pc_pass();
}
