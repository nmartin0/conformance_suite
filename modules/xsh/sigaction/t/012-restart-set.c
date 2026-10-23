/*
 * With SA_RESTART, an interrupted read() restarts.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.012
 * MUTANT    sigaction.restart_ignored
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

static volatile sig_atomic_t alarms;
static volatile sig_atomic_t rescue_w = -1;

/* Re-armed until the read gives up; after two, unblock a wrongly restarted read. */
static void on_alarm(int sig)
{
	(void)sig;
	alarms++;
	if (alarms >= 2 && rescue_w != -1) {
		(void)write(rescue_w, "x", 1);
		rescue_w = -1;
	}
	else
		(void)alarm(1);
}

int main(void)
{
	int p[2];
	ssize_t r;
	char c;

	pc_begin("XSH.sigaction.012");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	install(SIGALRM, on_alarm, SA_RESTART, NULL);
	rescue_w = p[1];
	(void)alarm(1);
	r = read(p[0], &c, 1);
	(void)alarm(0);
	pc_check(r != -1 || errno != EINTR, "read() failed with EINTR although SA_RESTART was set");
	pc_check(r == 1 && c == 'x', "read() returned %ld (%s)", (long)r, r == -1 ? pc_errname(errno) : "");
	pc_check(alarms >= 2, "read() returned before the signal (only %d alarms)", (int)alarms);
	pc_pass();
}
