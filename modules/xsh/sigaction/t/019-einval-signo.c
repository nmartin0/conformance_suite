/*
 * sigaction() for an invalid signal number fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.019
 * MUTANT    sigaction.einval_accepted
 */
#include "pc.h"
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Installs fn for sig with the given flags and mask (FAIL or UNRESOLVED by phase). */
static void install(int sig, void (*fn)(int), int flags, const sigset_t *mask)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = fn;
	sa.sa_flags = flags;
	if (mask != NULL)
		sa.sa_mask = *mask;
	else
		sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed: %s", sig, pc_errname(errno));
}

/* The same with a three-argument function and SA_SIGINFO. */
static void install_info(int sig, void (*fn)(int, siginfo_t *, void *), int flags)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_sigaction = fn;
	sa.sa_flags = flags | SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction(%d) failed: %s", sig, pc_errname(errno));
}

/* Blocks (how = SIG_BLOCK) or unblocks one signal. */
static void mask1(int how, int sig)
{
	sigset_t s;

	sigemptyset(&s);
	sigaddset(&s, sig);
	if (sigprocmask(how, &s, NULL) != 0)
		pc_unresolved("sigprocmask failed");
}

static void h(int sig)
{
	(void)sig;
}

/* A signal number above every valid one: SIGRTMAX is below sysconf(_SC_NSIG) (Issue 8); INT_MAX where that is missing. */
static int big_sig(void)
{
#ifdef _SC_NSIG
	long n = sysconf(_SC_NSIG);

	if (n > 0 && n <= INT_MAX)
		return (int)n;
#endif
	return INT_MAX;
}

int main(void)
{
	struct sigaction sa, o;
	int bad[3] = { 0, -1, 0 }, i;

	pc_begin("XSH.sigaction.019");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = h;
	sigemptyset(&sa.sa_mask);
	bad[2] = big_sig();	/* setup */
	pc_test();
	for (i = 0; i < 3; i++) {
		char what[64];
		(void)snprintf(what, sizeof what, "sigaction(%d, &act, NULL)", bad[i]);
		pc_expect_err(sigaction(bad[i], &sa, NULL), EINVAL, what);
		(void)snprintf(what, sizeof what, "sigaction(%d, NULL, &oact)", bad[i]);
		pc_expect_err(sigaction(bad[i], NULL, &o), EINVAL, what);
	}
	pc_pass();
}
