/*
 * pthread_cond_signal.c - conforming variants of pthread_cond_signal()
 * and pthread_cond_broadcast(), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include "pcvariant.h"

static void sleep_ms(long ms)
{
	struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };

	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/*
 * "wakes_all": pthread_cond_signal() unblocks every waiter. The page asks
 * it to unblock at least one of the blocked threads (and its RATIONALE
 * describes multiple awakenings), so unblocking all of them conforms.
 * "late": both functions take 200 ms before they unblock anyone. No
 * bound is set on how long a call takes or how soon an unblocked thread
 * runs.
 */
int pthread_cond_signal(pthread_cond_t *cv)
{
	const char *v = variant_of("pthread_cond_signal.");
	REAL(pthread_cond_signal);
	REAL(pthread_cond_broadcast);

	if (IS("wakes_all"))
		return real_pthread_cond_broadcast(cv);
	if (IS("late"))
		sleep_ms(200);
	return real_pthread_cond_signal(cv);
}

int pthread_cond_broadcast(pthread_cond_t *cv)
{
	const char *v = variant_of("pthread_cond_signal.");
	REAL(pthread_cond_broadcast);

	if (IS("late"))
		sleep_ms(200);
	return real_pthread_cond_broadcast(cv);
}
