/*
 * A signal sent to one waiting thread returns only that thread from sigwait().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigwait.005
 * MUTANT    sigwait.not_cleared
 * VARIANT   sigwait.action_dfl
 * VARIANT   sigwait.late
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

static volatile int got[2] = { 0, 0 };

static void *waiter(void *arg)
{
	sigset_t s = set_of(SIGUSR1, 0);
	int sig = 0, i = (int)(long)arg;

	if (sigwait(&s, &sig) == 0 && sig == SIGUSR1)
		got[i] = 1;
	return NULL;
}

int main(void)
{
	pthread_t t[2];
	struct timespec ts = { 0, 200000000 }, step = { 0, 10000000 };
	int i;

	pc_begin("XSH.sigwait.005");
	block(SIGUSR1);
	pc_test();
	if (!(pthread_create(&t[0], NULL, waiter, (void *)0L) == 0 &&
	    pthread_create(&t[1], NULL, waiter, (void *)1L) == 0))
		pc_unresolved("pthread_create failed");
	(void)nanosleep(&ts, NULL);	/* both are waiting */
	pc_check(pthread_kill(t[1], SIGUSR1) == 0, "pthread_kill failed");
	for (i = 0; i < 200 && !got[1]; i++)
		(void)nanosleep(&step, NULL);
	pc_check(got[1] == 1, "the thread the signal was sent to did not return within 2 s");
	(void)pthread_join(t[1], NULL);
	(void)nanosleep(&ts, NULL);
	pc_check(got[0] == 0, "the other waiting thread returned too");
	pc_check(pthread_kill(t[0], SIGUSR1) == 0, "pthread_kill failed");
	(void)pthread_join(t[0], NULL);
	pc_pass();
}
