/*
 * A thread suspended in sleep(), nanosleep() or clock_nanosleep() does not
 * stop the other threads from running to completion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.2-concurrent-execution.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

static volatile int sleeper_done;
static volatile int worker_done;
static int which;

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out: a suspended thread kept another one from running (case %d)", which);
}

static void *sleeper(void *arg)
{
	struct timespec ts = { 0, 300000000L }, rem;

	switch (*(int *)arg) {
	case 0:
		(void)sleep(1);
		break;
	case 1:
		while (nanosleep(&ts, &rem) == -1 && errno == EINTR)
			ts = rem;
		break;
	default:
		(void)clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
		break;
	}
	sleeper_done = 1;
	return NULL;
}

static void *worker(void *arg)
{
	volatile unsigned long n = 0;
	int i;

	(void)arg;
	for (i = 0; i < 100000; i++)
		n += (unsigned long)i;
	worker_done = (int)(n != 0);
	return NULL;
}

int main(void)
{
	pthread_t ts, tw;
	int k, r;
	struct timespec nap = { 0, 5000000L };

	pc_begin("XBD.4.2-concurrent-execution.001");
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(60);
	for (k = 0; k < 3; k++) {
		which = k;
		sleeper_done = 0;
		worker_done = 0;
		r = pthread_create(&ts, NULL, sleeper, &which);
		if (r != 0)
			pc_unresolved("pthread_create: %s", pc_errname(r));
		r = pthread_create(&tw, NULL, worker, NULL);
		if (r != 0)
			pc_unresolved("pthread_create: %s", pc_errname(r));
		while (!worker_done || !sleeper_done)
			(void)nanosleep(&nap, NULL);
		(void)pthread_join(tw, NULL);
		(void)pthread_join(ts, NULL);
	}
	pc_pass();
}
