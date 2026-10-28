/*
 * A notification function run for a sigevent whose attributes designate an
 * application-managed stack runs on that stack.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.034
 *
 * A one-shot timer with SIGEV_THREAD and an attributes object holding a
 * stack the test allocated (1 MiB, or the minimum size if larger) expires;
 * the address of an automatic variable of the notification function must
 * lie inside that region. The timer is never repeated and the stack is
 * never freed.
 */
#include "pcth.h"

#include <stdint.h>

static volatile uintptr_t where;

static void notify(union sigval v)
{
	char probe;

	(void)v;
	where = (uintptr_t)&probe;
	flag_set(0, 1);
}

int main(void)
{
	pthread_attr_t attr;
	struct sigevent sev;
	struct itimerspec its;
	timer_t tid;
	void *stk;
	size_t size = 1u << 20, align = (size_t)sysconf(_SC_PAGESIZE);
	int r;

	pc_begin("XSH.2.9-threads.034");
	guard(60);
#ifdef PTHREAD_STACK_MIN
	if (size < (size_t)PTHREAD_STACK_MIN)
		size = (size_t)PTHREAD_STACK_MIN;
#endif
	r = posix_memalign(&stk, align, size);
	NEED(r == 0, "posix_memalign failed");
	memset(stk, 0, size);
	NEED(pthread_attr_init(&attr) == 0, "pthread_attr_init failed");
	/* A joinable attributes object for a notification thread is undefined (XSH 2.4.2). */
	NEED(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) == 0, "pthread_attr_setdetachstate failed");
	r = pthread_attr_setstack(&attr, stk, size);
	NEED(r == 0, "pthread_attr_setstack failed: %s", pc_errname(r));
	memset(&sev, 0, sizeof sev);
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = notify;
	sev.sigev_notify_attributes = &attr;
	NEED(timer_create(CLOCK_MONOTONIC, &sev, &tid) == 0, "timer_create failed: %s", pc_errname(errno));
	memset(&its, 0, sizeof its);
	its.it_value.tv_nsec = 20 * 1000000L;
	pc_test();
	NEED(timer_settime(tid, 0, &its, NULL) == 0, "timer_settime failed: %s", pc_errname(errno));
	pc_check(wait_flag(0, 1, 10000), "the notification function was not called within 10 s");
	pc_check(where >= (uintptr_t)stk && where < (uintptr_t)stk + size,
	    "the notification function ran on a stack outside the region given in the attributes");
	(void)timer_delete(tid);
	pc_pass();
}
