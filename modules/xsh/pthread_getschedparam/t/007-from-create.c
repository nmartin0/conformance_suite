/*
 * A thread created with explicit scheduling attributes reports them, and
 * then the values of the latest pthread_setschedparam().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_getschedparam.007
 */
#include "schedh.h"
#include <stdatomic.h>

static _Atomic int go;

static void *waiter(void *arg)
{
	int i;

	(void)arg;
	for (i = 0; i < 2000 && !atomic_load(&go); i++) {
		struct timespec ts = { 0, 5000000L };

		nanosleep(&ts, NULL);
	}
	return NULL;
}

static int body(char *msg)
{
	pthread_attr_t at;
	struct sched_param sp;
	pthread_t t;
	int lo, hi, rlo, rhi, r, e;

	if ((r = sh_rng(SCHED_FIFO, &lo, &hi, msg)) != SH_PASS)
		return r;
	if ((r = sh_rng(SCHED_RR, &rlo, &rhi, msg)) != SH_PASS)
		return r;
	memset(&sp, 0, sizeof sp);
	sp.sched_priority = lo + 3;
	if (pthread_attr_init(&at) != 0 ||
	    pthread_attr_setinheritsched(&at, PTHREAD_EXPLICIT_SCHED) != 0 ||
	    pthread_attr_setschedpolicy(&at, SCHED_FIFO) != 0 ||
	    pthread_attr_setschedparam(&at, &sp) != 0) {
		SH_SAY(msg, "cannot prepare the thread attributes");
		return SH_UNRES;
	}
	if ((e = pthread_create(&t, &at, waiter, NULL)) != 0) {
		SH_SAY(msg, "pthread_create with SCHED_FIFO failed: %s", pc_errname(e));
		return SH_UNRES;
	}
	r = sh_texpect(t, SCHED_FIFO, lo + 3, msg);
	if (r == SH_PASS && (e = sh_tset(t, SCHED_RR, rhi)) != 0) {
		SH_SAY(msg, "pthread_setschedparam(RR, %d) failed: %s", rhi, pc_errname(e));
		r = SH_FAIL;
	}
	if (r == SH_PASS)
		r = sh_texpect(t, SCHED_RR, rhi, msg);
	atomic_store(&go, 1);
	(void)pthread_join(t, NULL);
	return r;
}

int main(void)
{
	pc_begin("XSH.pthread_getschedparam.007");
	pc_test();
	sh_run(body, 0);
	pc_pass();
}
