/*
 * pthread_kill() delivers the signal to the target thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_kill.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <string.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static atomic_int up, stop, handled;
static volatile pthread_t handler_thread;

static void on_signal(int sig)
{
	(void)sig;
	handler_thread = pthread_self();
	atomic_store(&handled, 1);
}

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

static void *target(void *arg)
{
	(void)arg;
	atomic_store(&up, 1);
	while (!atomic_load(&stop))
		sleep_ms(5);
	return NULL;
}

int main(void)
{
	struct sigaction sa;
	pthread_t t;
	int r, i;

	pc_begin("XSH.pthread_kill.001");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_signal;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed: %s", pc_errname(errno));
	NEED(pthread_create(&t, NULL, target, NULL) == 0, "pthread_create failed");
	for (i = 0; i < 2000 && !atomic_load(&up); i++)
		sleep_ms(5);
	NEED(atomic_load(&up), "the target thread did not start");
	pc_test();
	r = pthread_kill(t, SIGUSR1);
	pc_check(r == 0, "pthread_kill() returned %s", r == 0 ? "0" : pc_errname(r));
	for (i = 0; i < 2000 && !atomic_load(&handled); i++)
		sleep_ms(5);
	pc_check(atomic_load(&handled), "the SIGUSR1 handler did not run within 10 s");
	pc_check(pthread_equal(handler_thread, t) != 0, "the handler ran in a thread other than the target");
	pc_check(pthread_equal(handler_thread, pthread_self()) == 0, "the handler ran in the calling thread");
	atomic_store(&stop, 1);
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	pc_pass();
}
