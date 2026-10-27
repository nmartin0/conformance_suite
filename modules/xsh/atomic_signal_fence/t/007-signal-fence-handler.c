/*
 * A signal handler run on the same thread after atomic_signal_fence()
 * (release) and a relaxed store sees the data written before the fence
 * once it has read the stored value and run an acquire signal fence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_signal_fence.007
 */
#define PCAT_ID "XSH.atomic_signal_fence.007"
#include "pcat.h"
#ifndef PCAT_NONE

#include <signal.h>

#define N 2000

static int data;			/* ordinary memory, shared with the handler */
static atomic_int flag;
static volatile sig_atomic_t bad;
static volatile sig_atomic_t handled;

static void on_signal(int sig)
{
	int f;

	(void)sig;
	f = atomic_load_explicit(&flag, memory_order_relaxed);
	atomic_signal_fence(memory_order_acquire);
	if (f > 0 && data != f * 7 + 1)
		bad++;
	handled++;
}

int main(void)
{
	struct sigaction sa;
	int k;

	pc_begin(PCAT_ID);
	atomic_store(&flag, 0);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) != 0)
		pc_unresolved("sigaction failed");
	pc_test();
	for (k = 1; k <= N; k++) {
		data = k * 7 + 1;
		atomic_signal_fence(memory_order_release);
		atomic_store_explicit(&flag, k, memory_order_relaxed);
		if (raise(SIGUSR1) != 0)
			pc_unresolved("raise failed");
		if (handled != k)
			pc_unresolved("the handler did not run before raise() returned (round %d)", k);
	}
	pc_check(bad == 0, "the handler saw stale data in %d of %d rounds", (int)bad, N);
	pc_pass();
}
#endif
