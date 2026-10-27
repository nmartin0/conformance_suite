/*
 * PTHREAD_EXPLICIT_SCHED: the new thread gets the attributes object's policy and priority.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getinheritsched.004
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && _POSIX_THREAD_PRIORITY_SCHEDULING >= 0
struct seen {
	int r, policy, prio;
};

static void *routine(void *p)
{
	struct seen *s = p;
	struct sched_param sp;

	sp.sched_priority = -1;
	s->r = pthread_getschedparam(pthread_self(), &s->policy, &sp);
	s->prio = sp.sched_priority;
	return NULL;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	struct sched_param mine;
	pthread_attr_t a;
	pthread_t t;
	struct seen s = { -1, -1, -1 };
	int mypol, r;

	pc_begin("XSH.pthread_attr_getinheritsched.004");
	NEED(pthread_getschedparam(pthread_self(), &mypol, &mine) == 0, "pthread_getschedparam of the creator failed");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	NEED(pthread_attr_setinheritsched(&a, PTHREAD_EXPLICIT_SCHED) == 0, "setinheritsched(EXPLICIT) failed");
	NEED(pthread_attr_setschedpolicy(&a, mypol) == 0, "pthread_attr_setschedpolicy failed");
	NEED(pthread_attr_setschedparam(&a, &mine) == 0, "pthread_attr_setschedparam failed");
	pc_test();
	r = pthread_create(&t, &a, routine, &s);
	if (r == EAGAIN || r == EPERM || r == ENOTSUP)
		pc_unresolved("pthread_create() with the creator's own scheduling attributes failed with %s", pc_errname(r));
	pc_check(r == 0, "pthread_create() with PTHREAD_EXPLICIT_SCHED returned %s", rn(r));
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	NEED(s.r == 0, "pthread_getschedparam in the new thread returned %s", rn(s.r));
	pc_check(s.policy == mypol, "the new thread has policy %d, expected the attributes object's %d", s.policy, mypol);
	pc_check(s.prio == mine.sched_priority, "the new thread has priority %d, expected the attributes object's %d", s.prio,
	    mine.sched_priority);
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getinheritsched.004");
	pc_unsupported("_POSIX_THREAD_PRIORITY_SCHEDULING is not defined");
}
#endif
