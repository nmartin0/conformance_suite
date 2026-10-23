/*
 * sigpending() reports signals pending on the calling thread, not another thread's.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigpending.002
 * MUTANT    sigpending.empty
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

static pthread_t t2;
static volatile int phase, t2_has_thread, t2_has_proc;

static void *other(void *arg)
{
	sigset_t p;
	struct timespec ts = { 0, 10000000 };

	(void)arg;
	while (phase != 1)
		(void)nanosleep(&ts, NULL);
	t2_has_thread = sigpending(&p) == 0 && sigismember(&p, SIGUSR1) == 1;
	t2_has_proc = sigismember(&p, SIGUSR2) == 1;
	phase = 2;
	return NULL;
}

int main(void)
{
	sigset_t both = set_of(SIGUSR1, SIGUSR2, 0), p;
	struct timespec zero = { 0, 0 }, ts = { 0, 10000000 };

	pc_begin("XSH.sigpending.002");
	pc_check(pthread_sigmask(SIG_BLOCK, &both, NULL) == 0, "pthread_sigmask failed");
	pc_check(pthread_create(&t2, NULL, other, NULL) == 0, "pthread_create failed");
	pc_test();
	pc_check(pthread_kill(t2, SIGUSR1) == 0, "pthread_kill failed");
	pc_check(kill(getpid(), SIGUSR2) == 0, "kill failed");
	pc_check(sigpending(&p) == 0, "sigpending() failed");
	phase = 1;
	while (phase != 2)
		(void)nanosleep(&ts, NULL);
	(void)pthread_join(t2, NULL);
	pc_check(sigismember(&p, SIGUSR1) != 1, "a signal sent to another thread is pending in this thread's set");
	pc_check(sigismember(&p, SIGUSR2) == 1, "the process-directed signal is not in this thread's set");
	pc_check(t2_has_thread, "the signal sent to the thread is not in its own set");
	pc_check(t2_has_proc, "the process-directed signal is not in the other thread's set");
	(void)sigtimedwait(&both, NULL, &zero);
	pc_pass();
}
