/*
 * SA_RESETHAND clears SA_SIGINFO on entry to the handler.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.023
 * VARIANT   sigaction.resethand_nodefer
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

static volatile sig_atomic_t runs, was_dfl, info_clear;

static void h(int sig, siginfo_t *si, void *uc)
{
	struct sigaction cur;

	(void)si;
	(void)uc;
	if (sigaction(sig, NULL, &cur) == 0) {
		was_dfl = cur.sa_handler == SIG_DFL;
		info_clear = (cur.sa_flags & SA_SIGINFO) == 0;
	}
	runs++;
}

int main(void)
{
	pc_begin("XSH.sigaction.023");
	pc_test();
	install_info(SIGRTMIN, h, SA_RESETHAND);
	pc_check(kill(getpid(), SIGRTMIN) == 0, "kill failed");
	pc_check(runs == 1, "the handler ran %d times", (int)runs);
	pc_check(info_clear, "SA_SIGINFO was still set inside the handler");
	pc_pass();
}
