/*
 * With SA_NOCLDSTOP, a child's stop sends no SIGCHLD.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.008
 * MUTANT    sigaction.nocldstop_ignored
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
	sigset_t p;
	siginfo_t si;
	struct timespec ts = { 0, 100000000 };
	pid_t c;
	int pend;

	pc_begin("XSH.sigaction.008");
	pc_test();
	install(SIGCHLD, on_chld, SA_NOCLDSTOP, NULL);
	mask1(SIG_BLOCK, SIGCHLD);
	c = stopper();
	memset(&si, 0, sizeof si);
	if (waitid(P_PID, (id_t)c, &si, WSTOPPED | WNOWAIT) != 0) {
		(void)kill(c, SIGKILL);
		pc_unresolved("waitid(WSTOPPED) failed: %s", pc_errname(errno));
	}
	(void)nanosleep(&ts, NULL);	/* time for a late signal */
	pend = sigpending(&p) == 0 && sigismember(&p, SIGCHLD) == 1;
	(void)kill(c, SIGKILL);
	(void)waitpid(c, NULL, 0);
	pc_check(!pend, "SIGCHLD is pending after a child stopped, with SA_NOCLDSTOP set");
	pc_pass();
}
