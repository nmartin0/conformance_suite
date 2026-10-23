/*
 * raise() sends the signal to the calling thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.raise.002
 * MUTANT    raise.to_process
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h>
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

static _Thread_local volatile sig_atomic_t here;	/* set by the handler in its thread */
static volatile sig_atomic_t anywhere;

static void h(int sig)
{
	(void)sig;
	here = 1;
	anywhere = 1;
}

static volatile int go, stop, other_here;

/* A thread with SIGUSR1 unblocked, alive through the whole test. */
static void *other(void *arg)
{
	sigset_t a = set_of(SIGUSR1, 0);
	struct timespec ts = { 0, 10000000 };
	(void)arg;
	(void)pthread_sigmask(SIG_UNBLOCK, &a, NULL);
	go = 1;
	while (!stop)
		(void)nanosleep(&ts, NULL);
	other_here = here;
	return NULL;
}

int main(void)
{
	sigset_t a = set_of(SIGUSR1, 0), p;
	pthread_t t;
	struct timespec ts = { 0, 200000000 }, tick = { 0, 10000000 };

	pc_begin("XSH.raise.002");
	catch_sig(SIGUSR1, h);
	pc_check(pthread_create(&t, NULL, other, NULL) == 0, "pthread_create failed");
	while (!go)
		(void)nanosleep(&tick, NULL);
	pc_test();
	/* Unblocked here: the handler runs in this thread. */
	pc_check(raise(SIGUSR1) == 0, "raise failed");
	pc_check(here, "the handler did not run in the thread that called raise()");
	/* Blocked here: it stays pending for this thread, not for the other one. */
	here = 0;
	anywhere = 0;
	pc_check(pthread_sigmask(SIG_BLOCK, &a, NULL) == 0, "pthread_sigmask failed");
	pc_check(raise(SIGUSR1) == 0, "raise failed");
	(void)nanosleep(&ts, NULL);
	pc_check(!anywhere, "the signal raised by a thread that blocks it was delivered to another thread");
	pc_check(sigpending(&p) == 0 && sigismember(&p, SIGUSR1) == 1, "the raised signal is not pending");
	pc_check(pthread_sigmask(SIG_UNBLOCK, &a, NULL) == 0, "pthread_sigmask failed");
	pc_check(here, "unblocking did not deliver the pending signal to this thread");
	stop = 1;
	(void)pthread_join(t, NULL);
	pc_check(!other_here, "the handler ran in the other thread");
	pc_pass();
}
