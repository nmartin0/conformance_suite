/*
 * sigqueue() sends the signal with its value to another process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigqueue.001
 * MUTANT    sigqueue.value_lost
 * MUTANT    sigqueue.drops
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Installs fn for sig (FAIL or UNRESOLVED by phase). */
static void catch_sig(int sig, void (*fn)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed", sig);
}

/* A set holding the listed signals (end the list with 0). */
static sigset_t set_of(int first, ...)
{
	sigset_t s;
	va_list ap;
	int sig;

	sigemptyset(&s);
	va_start(ap, first);
	for (sig = first; sig != 0; sig = va_arg(ap, int))
		sigaddset(&s, sig);
	va_end(ap);
	return s;
}

static void block(int sig)
{
	sigset_t s = set_of(sig, 0);

	pc_check(pthread_sigmask(SIG_BLOCK, &s, NULL) == 0, "pthread_sigmask failed");
}

static long now_ms(void)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long)t.tv_sec * 1000L + t.tv_nsec / 1000000L;
}

/*
 * A child that sends sig to this process after ms milliseconds, then
 * exits. The caller reaps it with reap_sender().
 */
static pid_t send_later(int sig, long ms)
{
	pid_t parent = getpid(), c = fork();

	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		struct timespec ts;
		ts.tv_sec = ms / 1000;
		ts.tv_nsec = (ms % 1000) * 1000000L;
		while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
			;
		(void)kill(parent, sig);
		_exit(0);
	}
	return c;
}

static void reap_sender(pid_t c)
{
	while (waitpid(c, NULL, 0) == -1 && errno == EINTR)
		;
}

/* A child that reports the siginfo_t of the first SIGRTMIN it gets. */
static int kw = -1;

static void on_rt(int sig, siginfo_t *si, void *uc)
{
	char b[64];
	int n;

	(void)sig;
	(void)uc;
	n = snprintf(b, sizeof b, "%d %d %ld", si->si_code, si->si_value.sival_int, (long)si->si_pid);
	(void)write(kw, b, (size_t)n);
	_exit(0);
}

static pid_t rt_kid(int p[2])
{
	int ready[2];
	pid_t c;
	char ch;

	pc_check(pipe(ready) == 0, "pipe failed");
	c = fork();
	pc_check(c != -1, "fork failed");
	if (c == 0) {
		struct sigaction sa;
		kw = p[1];
		(void)close(p[0]);
		memset(&sa, 0, sizeof sa);
		sa.sa_sigaction = on_rt;
		sa.sa_flags = SA_SIGINFO;
		sigemptyset(&sa.sa_mask);
		if (sigaction(SIGRTMIN, &sa, NULL) != 0)
			_exit(99);
		(void)write(ready[1], "r", 1);
		for (;;)
			(void)pause();
	}
	(void)close(ready[1]);
	if (read(ready[0], &ch, 1) != 1)
		pc_unresolved("the child did not start");
	(void)close(ready[0]);
	return c;
}

int main(void)
{
	int p[2], code = -1, val = -1, st;
	long pid = -1;
	char b[64];
	ssize_t k;
	union sigval v;
	pid_t c;

	pc_begin("XSH.sigqueue.001");
	pc_check(pipe(p) == 0, "pipe failed");
	c = rt_kid(p);
	(void)close(p[1]);
	v.sival_int = 4321;
	pc_test();
	if (sigqueue(c, SIGRTMIN, v) != 0) {
		int e = errno;
		(void)kill(c, SIGKILL);
		(void)waitpid(c, NULL, 0);
		pc_fail("sigqueue() failed: %s", pc_errname(e));
	}
	{
		struct pollfd pf = { p[0], POLLIN, 0 };
		k = poll(&pf, 1, 5000) == 1 ? read(p[0], b, sizeof b - 1) : 0;
	}
	(void)kill(c, SIGKILL);
	(void)waitpid(c, &st, 0);
	pc_check(k > 0, "the child got no signal within 5 s");
	b[k] = '\0';
	pc_check(sscanf(b, "%d %d %ld", &code, &val, &pid) == 3, "bad report \"%s\"", b);
	pc_check(code == SI_QUEUE, "si_code is %d, expected SI_QUEUE", code);
	pc_check(val == 4321, "si_value is %d, expected 4321", val);
	pc_check(pid == (long)getpid(), "si_pid is %ld, expected %ld", pid, (long)getpid());
	pc_pass();
}
