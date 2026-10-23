/*
 * sigaction() reports the action previously installed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.001
 * MUTANT    sigaction.oact_wrong_handler
 * MUTANT    sigaction.oact_mask_lost
 * MUTANT    sigaction.oact_flags_lost
 * MUTANT    sigaction.query_resets
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

static volatile sig_atomic_t ran;

static void h(int sig)
{
	(void)sig;
	ran = 1;
}

int main(void)
{
	struct sigaction o;
	sigset_t m;

	pc_begin("XSH.sigaction.001");
	sigemptyset(&m);
	sigaddset(&m, SIGUSR2);
	pc_test();
	install(SIGUSR1, h, SA_RESTART, &m);
	memset(&o, 0, sizeof o);
	pc_expect_ok(sigaction(SIGUSR1, NULL, &o), "sigaction(SIGUSR1, NULL, &oact)");
	pc_check(o.sa_handler == h, "oact->sa_handler is not the installed handler");
	pc_check(sigismember(&o.sa_mask, SIGUSR2) == 1, "oact->sa_mask lost SIGUSR2");
	pc_check((o.sa_flags & SA_RESTART) != 0, "oact->sa_flags lost SA_RESTART");
	pc_check((o.sa_flags & SA_SIGINFO) == 0, "oact->sa_flags has SA_SIGINFO, which was not set");
	pc_check(kill(getpid(), SIGUSR1) == 0, "kill failed");
	pc_check(ran, "the handler did not run after the query: the query changed the action");
	pc_pass();
}
