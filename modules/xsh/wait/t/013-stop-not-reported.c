/*
 * A stopped child is not reported without WUNTRACED.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wait.013
 * MUTANT    wait.stop_reported_always
 * VARIANT   wait.status_encoding
 * VARIANT   wait.slow_stop
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
	pid_t c, r;
	int st;

	pc_begin("XSH.wait.013");
	guard(10);
	c = stopper();
	{
		/*
		 * Wait until it has stopped, however long that takes (nothing
		 * bounds it), without consuming the stop: WNOWAIT leaves the
		 * child waitable and does not change its state (XSH waitid()).
		 */
		siginfo_t si;

		memset(&si, 0, sizeof si);
		if (waitid(P_PID, (id_t)c, &si, WSTOPPED | WNOWAIT) != 0) {
			int e = errno;
			(void)kill(c, SIGKILL);
			pc_unresolved("waitid(WSTOPPED | WNOWAIT) failed: %s", pc_errname(e));
		}
	}
	pc_test();
	r = waitpid(c, &st, WNOHANG);
	pc_check(r == 0, "waitpid(WNOHANG) without WUNTRACED returned %ld for a stopped child", (long)r);
	r = waitpid(c, &st, WNOHANG | WUNTRACED);
	if (r != c || !WIFSTOPPED(st))
		pc_unresolved("the child had not stopped (the check above proves nothing)");
	(void)kill(c, SIGKILL);
	(void)waitpid(c, &st, 0);
	pc_pass();
}
