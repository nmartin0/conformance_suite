/*
 * With SA_SIGINFO, a sigqueue() value reaches the handler as si_value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.006
 * MUTANT    sigaction.siginfo_dropped
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
static siginfo_t info;

static void h(int sig, siginfo_t *si, void *uc)
{
	(void)sig;
	(void)uc;
	info = *si;
	ran = 1;
}

int main(void)
{
	union sigval v;

	pc_begin("XSH.sigaction.006");
	pc_test();
	install_info(SIGUSR1, h, 0);
	v.sival_int = 1234;
	pc_check(sigqueue(getpid(), SIGUSR1, v) == 0, "sigqueue failed: %s", pc_errname(errno));
	pc_check(ran, "the handler did not run");
	pc_check(info.si_code == SI_QUEUE, "si_code is %d, expected SI_QUEUE", info.si_code);
	pc_check(info.si_value.sival_int == 1234, "si_value is %d, expected 1234", info.si_value.sival_int);
	pc_pass();
}
