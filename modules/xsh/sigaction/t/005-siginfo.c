/*
 * With SA_SIGINFO, sa_sigaction gets a siginfo_t describing a kill().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.005
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
static int argsig;

static void h(int sig, siginfo_t *si, void *uc)
{
	(void)uc;
	argsig = sig;
	if (si != NULL)
		info = *si;
	ran = si != NULL ? 1 : 2;
}

int main(void)
{
	pc_begin("XSH.sigaction.005");
	pc_test();
	install_info(SIGUSR1, h, 0);
	pc_check(kill(getpid(), SIGUSR1) == 0, "kill failed");
	pc_check(ran != 0, "the handler did not run");
	pc_check(ran == 1, "the handler got a null siginfo_t pointer");
	pc_check(argsig == SIGUSR1, "the first argument is %d, expected SIGUSR1", argsig);
	pc_check(info.si_signo == SIGUSR1, "si_signo is %d, expected SIGUSR1", info.si_signo);
	pc_check(info.si_code == SI_USER, "si_code is %d, expected SI_USER for kill()", info.si_code);
	pc_check(info.si_pid == getpid(), "si_pid is %ld, expected %ld", (long)info.si_pid, (long)getpid());
	pc_check(info.si_uid == getuid(), "si_uid is %ld, expected %ld", (long)info.si_uid, (long)getuid());
	pc_pass();
}
