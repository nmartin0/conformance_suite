/*
 * A lock that is possible at once does not time out.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutex_clocklock.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

struct try_job {
	pthread_mutex_t *m;
	int r;
};

static void *try_main(void *p)
{
	struct try_job *j = p;

	j->r = pthread_mutex_trylock(j->m);
	if (j->r == 0)
		(void)pthread_mutex_unlock(j->m);
	return NULL;
}

int main(void)
{
	static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	struct timespec past = { 0, 0 };
	struct try_job j;
	pthread_t t;
	int r, k;

	pc_begin("XSH.pthread_mutex_clocklock.001");
	j.m = &m;
	pc_test();
	for (k = 0; k < 3; k++) {
		const char *what = k == 0 ? "pthread_mutex_timedlock()" : k == 1 ? "pthread_mutex_clocklock(CLOCK_REALTIME)"
		    : "pthread_mutex_clocklock(CLOCK_MONOTONIC)";

		if (k == 0)
			r = pthread_mutex_timedlock(&m, &past);
		else
			r = pthread_mutex_clocklock(&m, k == 1 ? CLOCK_REALTIME : CLOCK_MONOTONIC, &past);
		pc_check(r == 0, "%s of an unlocked mutex with a past time returned %s", what, rn(r));
		j.r = -1;
		NEED(pthread_create(&t, NULL, try_main, &j) == 0, "pthread_create failed");
		NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
		pc_check(j.r == EBUSY, "after %s, another thread's trylock returned %s, expected EBUSY", what, rn(j.r));
		r = pthread_mutex_unlock(&m);
		pc_check(r == 0, "pthread_mutex_unlock() returned %s", rn(r));
	}
	pc_pass();
}
