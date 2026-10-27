/*
 * pthread_kill() with signal number 0 sends nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_kill.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <string.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static atomic_int handled;

static void on_signal(int sig)
{
	(void)sig;
	atomic_store(&handled, 1);
}

int main(void)
{
	struct sigaction sa;
	struct timespec ts = { 0, 200000000L };
	int r;

	pc_begin("XSH.pthread_kill.002");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed: %s", pc_errname(errno));
	NEED(sigaction(SIGUSR2, &sa, NULL) == 0, "sigaction failed: %s", pc_errname(errno));
	pc_test();
	r = pthread_kill(pthread_self(), 0);
	pc_check(r == 0 || r == EINVAL, "pthread_kill(self, 0) returned %s, expected 0 or EINVAL", pc_errname(r));
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
	pc_check(atomic_load(&handled) == 0, "a signal handler ran after pthread_kill() with signal number 0");
	pc_pass();
}
