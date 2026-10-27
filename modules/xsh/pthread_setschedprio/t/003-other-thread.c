/*
 * pthread_setschedprio() on another thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_setschedprio.003
 */
#include "schedh.h"
#include <stdatomic.h>

static _Atomic int go, verdict;
static int want_prio;

static void *waiter(void *arg)
{
	char msg[200];
	int i;

	(void)arg;
	for (i = 0; i < 2000 && !atomic_load(&go); i++) {
		struct timespec ts = { 0, 5000000L };

		nanosleep(&ts, NULL);
	}
	atomic_store(&verdict, 1 + sh_texpect(pthread_self(), SCHED_RR, want_prio, msg));
	return NULL;
}

static int body(char *msg)
{
	pthread_t t;
	int lo, hi, r, e;

	if ((r = sh_rng(SCHED_RR, &lo, &hi, msg)) != SH_PASS)
		return r;
	want_prio = lo + 4;
	if ((e = pthread_create(&t, NULL, waiter, NULL)) != 0) {
		SH_SAY(msg, "pthread_create failed: %s", pc_errname(e));
		return SH_UNRES;
	}
	r = sh_tenter(t, SCHED_RR, lo + 1, msg);
	if (r == SH_PASS && (e = pthread_setschedprio(t, lo + 4)) != 0) {
		SH_SAY(msg, "pthread_setschedprio(thread, %d) failed: %s", lo + 4, pc_errname(e));
		r = SH_FAIL;
	}
	if (r == SH_PASS)
		r = sh_texpect(t, SCHED_RR, lo + 4, msg);
	atomic_store(&go, 1);
	(void)pthread_join(t, NULL);
	if (r != SH_PASS)
		return r;
	if (atomic_load(&verdict) != 1 + SH_PASS) {
		SH_SAY(msg, "the thread did not see its own new priority");
		return SH_FAIL;
	}
	return SH_PASS;
}

int main(void)
{
	pc_begin("XSH.pthread_setschedprio.003");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
