/*
 * Passing sigaction()'s report of a signal() action back restores it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.017
 * MUTANT    sigaction.oact_wrong_handler
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

static volatile sig_atomic_t which;

static void h1(int sig) { (void)sig; which = 1; }
static void h2(int sig) { (void)sig; which = 2; }

int main(void)
{
	struct sigaction o;

	pc_begin("XSH.sigaction.017");
	pc_check(signal(SIGUSR1, h1) != SIG_ERR, "signal failed");
	pc_test();
	pc_expect_ok(sigaction(SIGUSR1, NULL, &o), "sigaction(SIGUSR1, NULL, &oact)");
	install(SIGUSR1, h2, 0, NULL);
	pc_expect_ok(sigaction(SIGUSR1, &o, NULL), "sigaction() with the reported action");
	pc_check(kill(getpid(), SIGUSR1) == 0, "kill failed");
	pc_check(which == 1, which == 2 ? "the later handler still runs" : "no handler ran");
	pc_pass();
}
