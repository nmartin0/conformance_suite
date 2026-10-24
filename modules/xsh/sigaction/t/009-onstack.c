/*
 * XSI: with SA_ONSTACK the handler runs on the alternate stack, without it on the current one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.009
 * MUTANT    sigaction.onstack_ignored
 * MUTANT    sigaction.onstack_always
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

static char stack_area[65536 + 32768];	/* more than MINSIGSTKSZ on known systems */
static volatile sig_atomic_t ran, on_alt;

static volatile sig_atomic_t ss_on;

static void h(int sig)
{
	char here;
	uintptr_t p = (uintptr_t)&here, lo = (uintptr_t)stack_area;
	stack_t cur;

	(void)sig;
	on_alt = p >= lo && p < lo + sizeof stack_area;
	ss_on = sigaltstack(NULL, &cur) == 0 && (cur.ss_flags & SS_ONSTACK) != 0;
	ran = 1;
}

int main(void)
{
	stack_t ss;

	pc_begin("XSH.sigaction.009");
	ss.ss_sp = stack_area;
	ss.ss_size = sizeof stack_area;
	ss.ss_flags = 0;
#ifdef MINSIGSTKSZ
	if (sizeof stack_area < (size_t)MINSIGSTKSZ)
		pc_unresolved("MINSIGSTKSZ is larger than the test's stack");
#endif
	pc_check(sigaltstack(&ss, NULL) == 0, "sigaltstack failed: %s", pc_errname(errno));
	pc_test();
	install(SIGUSR1, h, SA_ONSTACK, NULL);
	install(SIGUSR2, h, 0, NULL);
	pc_check(kill(getpid(), SIGUSR1) == 0 && ran, "the SA_ONSTACK handler did not run");
	pc_check(on_alt, "the SA_ONSTACK handler did not run on the alternate stack");
	ran = 0;
	pc_check(kill(getpid(), SIGUSR2) == 0 && ran, "the other handler did not run");
	pc_check(!on_alt, "a handler without SA_ONSTACK ran on the alternate stack");
	pc_pass();
}
