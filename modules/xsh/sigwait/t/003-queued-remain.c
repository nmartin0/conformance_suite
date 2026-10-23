/*
 * sigwait() takes one queued signal; the rest stay queued.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigwait.003
 * MUTANT    sigwait.drains_all
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

/*
 * A child that sends sig to this process after ms milliseconds and then
 * every `every' ms, until stop_sender() kills it (so a signal sent before
 * the caller started waiting cannot leave it waiting for ever).
 */
static pid_t send_repeat(int sig, long ms, long every)
{
	pid_t parent = getpid(), c = fork();

	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		struct timespec ts;
		ts.tv_sec = ms / 1000;
		ts.tv_nsec = (ms % 1000) * 1000000L;
		for (;;) {
			while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
				;
			if (kill(parent, sig) != 0)
				_exit(0);
			ts.tv_sec = every / 1000;
			ts.tv_nsec = (every % 1000) * 1000000L;
		}
	}
	return c;
}

static void stop_sender(pid_t c)
{
	(void)kill(c, SIGKILL);
	reap_sender(c);
}

/* A watchdog: a caught SIGALRM after s seconds sets watchdog_fired. */
static volatile sig_atomic_t watchdog_fired;

static void on_watchdog(int sig)
{
	(void)sig;
	watchdog_fired = 1;
}

static void watchdog(unsigned s)
{
	catch_sig(SIGALRM, on_watchdog);
	(void)alarm(s);
}

/* Waits for a byte on fd (a child saying it is about to block). */
static void await_ready(int fd)
{
	char ch;

	if (read(fd, &ch, 1) != 1)
		pc_unresolved("the child did not start");
}

static void hi(int sig, siginfo_t *si, void *uc)
{
	(void)sig;
	(void)si;
	(void)uc;
}

int main(void)
{
	sigset_t s = set_of(SIGRTMIN, 0), p;
	struct sigaction sa;
	union sigval v;
	int sig = 0;

	pc_begin("XSH.sigwait.003");
	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = hi;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGRTMIN, &sa, NULL) == 0, "sigaction failed");
	block(SIGRTMIN);
	v.sival_int = 1;
	pc_check(sigqueue(getpid(), SIGRTMIN, v) == 0 && sigqueue(getpid(), SIGRTMIN, v) == 0, "sigqueue failed");
	pc_test();
	pc_check(sigwait(&s, &sig) == 0 && sig == SIGRTMIN, "sigwait() did not return SIGRTMIN");
	pc_check(sigpending(&p) == 0 && sigismember(&p, SIGRTMIN) == 1, "the second queued SIGRTMIN is gone");
	pc_check(sigwait(&s, &sig) == 0 && sig == SIGRTMIN, "sigwait() did not return the second SIGRTMIN");
	pc_pass();
}
