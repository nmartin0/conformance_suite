/*
 * sched_yield() returns 0 in a thread other than the initial one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_yield.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sched.h>

static void *worker(void *arg)
{
	int i, *bad = arg;

	for (i = 0; i < 50; i++)
		if (sched_yield() != 0)
			*bad = i + 1;
	return NULL;
}

int main(void)
{
	pthread_t t;
	int bad = 0, e;

	pc_begin("XSH.sched_yield.002");
	pc_test();
	e = pthread_create(&t, NULL, worker, &bad);
	if (e != 0)
		pc_unresolved("pthread_create failed: %s", pc_errname(e));
	e = pthread_join(t, NULL);
	if (e != 0)
		pc_unresolved("pthread_join failed: %s", pc_errname(e));
	pc_check(bad == 0, "call %d in the thread did not return 0", bad);
	pc_pass();
}
