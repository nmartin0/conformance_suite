/*
 * Catching or ignoring SIGKILL or SIGSTOP fails with EINVAL and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.018
 * MUTANT    sigaction.einval_accepted
 * VARIANT   sigaction.dfl_uncatchable_ok
 */
#include "pc.h"
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

int main(void)
{
	struct sigaction sa, o;
	int sigs[2] = { SIGKILL, SIGSTOP }, i, k;

	pc_begin("XSH.sigaction.018");
	memset(&sa, 0, sizeof sa);
	sigemptyset(&sa.sa_mask);
	pc_test();
	for (i = 0; i < 2; i++)
		for (k = 0; k < 2; k++) {
			sa.sa_handler = k ? SIG_IGN : h;
			pc_expect_err(sigaction(sigs[i], &sa, NULL), EINVAL,
			    i ? (k ? "ignore SIGSTOP" : "catch SIGSTOP") : (k ? "ignore SIGKILL" : "catch SIGKILL"));
			pc_check(sigaction(sigs[i], NULL, &o) == 0 && o.sa_handler == SIG_DFL,
			    "the %s action changed after the failed call", i ? "SIGSTOP" : "SIGKILL");
		}
	pc_pass();
}
