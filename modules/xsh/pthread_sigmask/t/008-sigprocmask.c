/*
 * sigprocmask() does the same, returning -1 and setting errno on error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_sigmask.008
 * MUTANT    pthread_sigmask.block_replaces
 * MUTANT    pthread_sigmask.setmask_union
 * MUTANT    pthread_sigmask.einval_accepted
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

	if (sigprocmask(SIG_BLOCK, NULL, &cur) != 0)
		pc_unresolved("sigprocmask query failed");
	return sigismember(&cur, sig) == 1;
}

int main(void)
{
	sigset_t a = set_of(SIGUSR1, 0), b = set_of(SIGUSR2, 0), o;

	pc_begin("XSH.pthread_sigmask.008");
	pc_test();
	pc_check(sigprocmask(SIG_SETMASK, &a, NULL) == 0, "SIG_SETMASK failed");
	pc_check(sigprocmask(SIG_BLOCK, &b, &o) == 0, "SIG_BLOCK failed");
	pc_check(sigismember(&o, SIGUSR1) == 1 && sigismember(&o, SIGUSR2) != 1, "oset is wrong");
	pc_check(blocked(SIGUSR1) && blocked(SIGUSR2), "SIG_BLOCK did not form the union");
	pc_check(sigprocmask(SIG_UNBLOCK, &a, NULL) == 0, "SIG_UNBLOCK failed");
	pc_check(!blocked(SIGUSR1) && blocked(SIGUSR2), "SIG_UNBLOCK did not remove SIGUSR1 only");
	pc_check(sigprocmask(SIG_SETMASK, &a, NULL) == 0, "SIG_SETMASK failed");
	pc_check(blocked(SIGUSR1) && !blocked(SIGUSR2), "SIG_SETMASK did not replace the mask");
	pc_expect_err(sigprocmask(-12345, &b, NULL), EINVAL, "sigprocmask(<invalid how>)");
	pc_check(blocked(SIGUSR1) && !blocked(SIGUSR2), "the failed call changed the mask");
	pc_pass();
}
