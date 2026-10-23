/*
 * A pending signal that gets unblocked is delivered before the call returns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_sigmask.005
 * MUTANT    pthread_sigmask.unblock_drops
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

static volatile sig_atomic_t ran;

static void h(int sig)
{
	(void)sig;
	ran = 1;
}

int main(void)
{
	sigset_t a = set_of(SIGUSR1, 0);

	pc_begin("XSH.pthread_sigmask.005");
	catch_sig(SIGUSR1, h);
	pc_check(pthread_sigmask(SIG_BLOCK, &a, NULL) == 0, "setup failed");
	pc_check(kill(getpid(), SIGUSR1) == 0 && !ran, "SIGUSR1 was not held back while blocked");
	pc_test();
	pc_check(pthread_sigmask(SIG_UNBLOCK, &a, NULL) == 0, "pthread_sigmask(SIG_UNBLOCK) failed");
	pc_check(ran, "the pending SIGUSR1 had not been delivered when pthread_sigmask() returned");
	pc_pass();
}
