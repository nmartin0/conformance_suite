/*
 * Without SA_NOCLDSTOP, a child's stop sends SIGCHLD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.007
 * MUTANT    sigaction.nocldstop_always
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

static void on_chld(int sig)
{
	(void)sig;
}

/* A child that stops itself and then waits to be killed. */
static pid_t stopper(void)
{
	pid_t c = fork();

	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		(void)raise(SIGSTOP);
		for (;;)
			(void)pause();
	}
	return c;
}

int main(void)
{
	sigset_t s;
	struct timespec limit = { 5, 0 };
	pid_t c;
	int r;

	pc_begin("XSH.sigaction.007");
	pc_test();
	install(SIGCHLD, on_chld, 0, NULL);
	mask1(SIG_BLOCK, SIGCHLD);
	sigemptyset(&s);
	sigaddset(&s, SIGCHLD);
	c = stopper();
	r = sigtimedwait(&s, NULL, &limit);
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
	pc_check(r == SIGCHLD, "no SIGCHLD within 5 s of the child stopping (%s)", r == -1 ? pc_errname(errno) : "?");
	pc_pass();
}
