/*
 * SIGKILL and SIGSTOP in sa_mask are not blocked during the handler.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.003
 * MUTANT    sigaction.mask_ignored
 * MUTANT    sigaction.kill_in_mask_einval
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

static volatile sig_atomic_t ran, kill_in, stop_in, usr2_in;

static void h(int sig)
{
	sigset_t cur;

	(void)sig;
	if (sigprocmask(SIG_BLOCK, NULL, &cur) == 0) {
		kill_in = sigismember(&cur, SIGKILL) == 1;
		stop_in = sigismember(&cur, SIGSTOP) == 1;
		usr2_in = sigismember(&cur, SIGUSR2) == 1;
	}
	ran = 1;
}

int main(void)
{
	struct sigaction sa;

	pc_begin("XSH.sigaction.003");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = h;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGKILL);
	sigaddset(&sa.sa_mask, SIGSTOP);
	sigaddset(&sa.sa_mask, SIGUSR2);
	pc_test();
	pc_expect_ok(sigaction(SIGUSR1, &sa, NULL), "sigaction() with SIGKILL and SIGSTOP in sa_mask");
	pc_check(kill(getpid(), SIGUSR1) == 0, "kill failed");
	pc_check(ran, "the handler did not run");
	pc_check(!kill_in, "SIGKILL was blocked during the handler");
	pc_check(!stop_in, "SIGSTOP was blocked during the handler");
	pc_check(usr2_in, "SIGUSR2 from sa_mask was not blocked during the handler");
	pc_pass();
}
