/*
 * With SA_NODEFER, the signal is not blocked during its handler unless in sa_mask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.015
 * MUTANT    sigaction.nodefer_ignored
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

static volatile sig_atomic_t ran, self_in, usr2_in;

static void h(int sig)
{
	sigset_t cur;

	if (sigprocmask(SIG_BLOCK, NULL, &cur) == 0) {
		self_in = sigismember(&cur, sig) == 1;
		usr2_in = sigismember(&cur, SIGUSR2) == 1;
	}
	ran = 1;
}

int main(void)
{
	sigset_t m;

	pc_begin("XSH.sigaction.015");
	pc_test();
	install(SIGUSR1, h, SA_NODEFER, NULL);
	pc_check(raise(SIGUSR1) == 0 && ran, "the handler did not run");
	pc_check(!self_in, "SIGUSR1 was blocked during its handler although SA_NODEFER was set");
	ran = 0;
	sigemptyset(&m);
	sigaddset(&m, SIGUSR1);
	install(SIGUSR1, h, SA_NODEFER, &m);
	pc_check(raise(SIGUSR1) == 0 && ran, "the handler did not run");
	pc_check(self_in, "SIGUSR1 in sa_mask was not blocked during its handler with SA_NODEFER");
	pc_pass();
}
