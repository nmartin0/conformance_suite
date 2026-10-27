/*
 * The clock of another thread, obtained by the caller, reads at least what
 * that thread itself had read from its own clock.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getcpuclockid.002
 */
#include "cpuh.h"
#include <pthread.h>
#include <stdatomic.h>

static _Atomic long long own;	/* the thread's reading, -1 until published */
static _Atomic int ready, go;

static void *worker(void *arg)
{
	clockid_t id;
	int i;

	(void)arg;
	if (pthread_getcpuclockid(pthread_self(), &id) != 0 || cpu_burn(id) == 0)
		atomic_store(&own, -2);
	else
		atomic_store(&own, cpu_ns(id));
	atomic_store(&ready, 1);
	for (i = 0; i < 2000 && !atomic_load(&go); i++) {
		struct timespec ts = { 0, 5000000L };

		nanosleep(&ts, NULL);
	}
	return NULL;
}

int main(void)
{
	pthread_t t;
	clockid_t id;
	long long mine, theirs, again;
	int e, i;

	pc_begin("XSH.pthread_getcpuclockid.002");
	atomic_store(&own, -1);
	e = pthread_create(&t, NULL, worker, NULL);
	if (e != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(e));
	for (i = 0; i < 2000 && !atomic_load(&ready); i++) {
		struct timespec ts = { 0, 5000000L };

		nanosleep(&ts, NULL);
	}
	mine = atomic_load(&own);
	if (!atomic_load(&ready) || mine < 0) {
		atomic_store(&go, 1);
		pthread_join(t, NULL);
		pc_unresolved("the helper thread could not use its own CPU-time clock");
	}
	pc_test();
	e = pthread_getcpuclockid(t, &id);
	if (e != 0) {
		atomic_store(&go, 1);
		pthread_join(t, NULL);
		pc_fail("pthread_getcpuclockid(other thread) failed: %s", pc_errname(e));
	}
	theirs = cpu_ns(id);
	again = cpu_ns(id);
	atomic_store(&go, 1);
	pthread_join(t, NULL);
	pc_check(theirs >= 0 && again >= 0, "clock_gettime on the other thread's clock failed");
	pc_check(theirs >= mine, "the clock reads %lld ns, below the %lld ns the thread had read from it", theirs, mine);
	pc_check(again >= theirs, "second read %lld ns is below the first %lld ns", again, theirs);
	pc_pass();
}
