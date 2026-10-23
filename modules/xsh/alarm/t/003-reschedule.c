/*
 * A new alarm() replaces the pending one and returns the seconds left.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.alarm.003
 * MUTANT    alarm.no_reschedule
 * MUTANT    alarm.returns_zero
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

/* Waits up to ms for SIGALRM (blocked); returns the delay in ms, or -1. */
static long await_alrm(long t0, long ms)
{
	sigset_t s = set_of(SIGALRM, 0);
	struct timespec to;

	to.tv_sec = ms / 1000;
	to.tv_nsec = (ms % 1000) * 1000000L;
	if (sigtimedwait(&s, NULL, &to) != SIGALRM)
		return -1;
	return now_ms() - t0;
}

int main(void)
{
	struct timespec one = { 1, 0 };
	long t0, dt;
	unsigned r;

	pc_begin("XSH.alarm.003");
	block(SIGALRM);
	pc_test();
	(void)alarm(10);
	(void)nanosleep(&one, NULL);
	t0 = now_ms();
	r = alarm(2);
	dt = await_alrm(t0, 6000);
	(void)alarm(0);
	pc_check(r >= 8 && r <= 10, "alarm(2) returned %u, expected the 8-10 seconds left", r);
	pc_check(dt != -1, "no SIGALRM within 6 s of alarm(2): the first alarm was not replaced");
	pc_check(dt >= 1990, "SIGALRM came %ld ms after alarm(2), before 2 s", dt);
	pc_pass();
}
