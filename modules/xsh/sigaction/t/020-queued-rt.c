/*
 * With SA_SIGINFO, sigqueue()d occurrences of a realtime signal queue in FIFO order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigaction.020
 * VARIANT   sigaction.late_delivery
 * VARIANT   sigaction.sigqueue_max_32
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

static volatile sig_atomic_t n;
static int vals[8];

static void h(int sig, siginfo_t *si, void *uc)
{
	(void)sig;
	(void)uc;
	if (n < 8)
		vals[n] = si->si_value.sival_int;
	n++;
}

/*
 * Queues values 1, 2, 3 for sig while it is blocked, then unblocks and
 * blocks it again until three have been delivered (at least one is
 * delivered by each unblocking), or ten tries have passed.
 */
static void queue_three(int sig)
{
	union sigval v;
	int i;

	install_info(sig, h, 0);
	mask1(SIG_BLOCK, sig);
	for (i = 1; i <= 3; i++) {
		v.sival_int = i;
		if (sigqueue(getpid(), sig, v) != 0)
			pc_fail("sigqueue %d failed: %s", i, pc_errname(errno));
	}
	for (i = 0; i < 10 && n < 3; i++) {
		mask1(SIG_UNBLOCK, sig);
		mask1(SIG_BLOCK, sig);
	}
	mask1(SIG_UNBLOCK, sig);
}

int main(void)
{
	pc_begin("XSH.sigaction.020");
	pc_test();
	queue_three(SIGRTMIN);
	pc_check(n == 3, "%d deliveries for 3 queued signals", (int)n);
	pc_check(vals[0] == 1 && vals[1] == 2 && vals[2] == 3, "delivered in the order %d, %d, %d", vals[0], vals[1], vals[2]);
	pc_pass();
}
