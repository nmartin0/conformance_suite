/*
 * Each thread has its own signal mask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_sigmask.009
 * MUTANT    pthread_sigmask.block_ignored
 * VARIANT   pthread_sigmask.how_values
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

static int blocked(int sig)
{
	sigset_t cur;

	if (pthread_sigmask(SIG_BLOCK, NULL, &cur) != 0)
		pc_unresolved("pthread_sigmask query failed");
	return sigismember(&cur, sig) == 1;
}

static volatile int other_blocked = -1, inherited = -1;

static void *worker(void *arg)
{
	sigset_t a = set_of(SIGUSR1, 0);
	(void)arg;
	inherited = blocked(SIGUSR2);
	if (pthread_sigmask(SIG_BLOCK, &a, NULL) != 0)
		other_blocked = -2;
	else
		other_blocked = blocked(SIGUSR1);
	return NULL;
}

int main(void)
{
	sigset_t b = set_of(SIGUSR2, 0);
	pthread_t t;

	pc_begin("XSH.pthread_sigmask.009");
	pc_check(pthread_sigmask(SIG_BLOCK, &b, NULL) == 0, "pthread_sigmask failed");
	pc_test();
	pc_check(pthread_create(&t, NULL, worker, NULL) == 0, "pthread_create failed");
	pc_check(pthread_join(t, NULL) == 0, "pthread_join failed");
	if (inherited != 1)
		pc_unresolved("the new thread did not start with its creator's mask (a pthread_create() matter)");
	pc_check(other_blocked == 1, "the thread could not block SIGUSR1 in its own mask");
	pc_check(!blocked(SIGUSR1), "blocking SIGUSR1 in another thread changed this thread's mask");
	pc_pass();
}
