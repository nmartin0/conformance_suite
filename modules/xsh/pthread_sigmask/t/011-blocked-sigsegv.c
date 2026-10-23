/*
 * SIGSEGV sent with kill() while blocked stays pending like any signal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_sigmask.011
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

int main(void)
{
	int sigs[4] = { SIGSEGV, SIGFPE, SIGILL, SIGBUS }, i;
	struct timespec zero = { 0, 0 };

	pc_begin("XSH.pthread_sigmask.011");
	pc_test();
	for (i = 0; i < 4; i++) {
		sigset_t one = set_of(sigs[i], 0), p;
		pc_check(pthread_sigmask(SIG_BLOCK, &one, NULL) == 0, "pthread_sigmask failed");
		pc_check(kill(getpid(), sigs[i]) == 0, "kill failed");
		pc_check(sigpending(&p) == 0 && sigismember(&p, sigs[i]) == 1, "signal %d sent while blocked is not pending", sigs[i]);
		pc_check(sigtimedwait(&one, NULL, &zero) == sigs[i], "signal %d could not be accepted", sigs[i]);
	}
	pc_pass();
}
