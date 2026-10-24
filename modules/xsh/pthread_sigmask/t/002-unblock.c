/*
 * SIG_UNBLOCK removes the set from the mask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_sigmask.002
 * MUTANT    pthread_sigmask.unblock_ignored
 * MUTANT    pthread_sigmask.unblock_all
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

int main(void)
{
	sigset_t a = set_of(SIGUSR1, SIGUSR2, 0), b = set_of(SIGUSR1, 0);

	pc_begin("XSH.pthread_sigmask.002");
	pc_check(pthread_sigmask(SIG_SETMASK, &a, NULL) == 0, "setup failed");
	pc_test();
	pc_check(pthread_sigmask(SIG_UNBLOCK, &b, NULL) == 0, "pthread_sigmask(SIG_UNBLOCK) failed");
	pc_check(!blocked(SIGUSR1), "SIGUSR1 is still blocked after SIG_UNBLOCK");
	pc_check(blocked(SIGUSR2), "SIGUSR2 was unblocked too");
	pc_pass();
}
