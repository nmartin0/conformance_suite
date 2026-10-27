/*
 * pthread_kill() of a terminated, not yet joined thread is not an error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_kill.004
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static atomic_int finished;

static void *quick(void *arg)
{
	atomic_store(&finished, 1);
	return arg;
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

int main(void)
{
	pthread_t t;
	int r, i;

	pc_begin("XSH.pthread_kill.004");
	NEED(signal(SIGUSR1, SIG_IGN) != SIG_ERR, "signal(SIGUSR1, SIG_IGN) failed");
	NEED(pthread_create(&t, NULL, quick, NULL) == 0, "pthread_create failed");
	for (i = 0; i < 2000 && !atomic_load(&finished); i++)
		sleep_ms(5);
	NEED(atomic_load(&finished), "the thread did not run");
	sleep_ms(300);	/* let it terminate; it stays joinable */
	pc_test();
	r = pthread_kill(t, SIGUSR1);
	pc_check(r == 0, "pthread_kill() of a terminated, unjoined thread returned %s", r == 0 ? "0" : pc_errname(r));
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	pc_pass();
}
