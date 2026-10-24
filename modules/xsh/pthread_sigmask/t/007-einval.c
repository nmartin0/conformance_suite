/*
 * An invalid how gives EINVAL (returned, not in errno) and leaves the mask alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_sigmask.007
 * MUTANT    pthread_sigmask.einval_accepted
 * MUTANT    pthread_sigmask.returns_minus1
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

/*
 * A how value that is none of SIG_BLOCK, SIG_UNBLOCK and SIG_SETMASK. XBD
 * <signal.h> gives only their names, so no fixed number is surely
 * invalid.
 */
static int bad_how(void)
{
	int h = -12345;

	while (h == SIG_BLOCK || h == SIG_UNBLOCK || h == SIG_SETMASK)
		h++;
	return h;
}

int main(void)
{
	sigset_t a = set_of(SIGUSR1, 0), b = set_of(SIGUSR2, 0);
	int r;

	pc_begin("XSH.pthread_sigmask.007");
	pc_check(pthread_sigmask(SIG_SETMASK, &a, NULL) == 0, "setup failed");
	pc_test();
	r = pthread_sigmask(bad_how(), &b, NULL);
	pc_check(r != -1, "pthread_sigmask() returned -1; it returns the error number");
	pc_check(r == EINVAL, "pthread_sigmask() with an invalid how returned %d, expected EINVAL", r);
	pc_check(blocked(SIGUSR1) && !blocked(SIGUSR2), "the failed call changed the mask");
	pc_pass();
}
