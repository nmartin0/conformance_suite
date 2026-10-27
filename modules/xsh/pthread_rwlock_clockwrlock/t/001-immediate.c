/*
 * A lock that is possible at once does not time out.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_rwlock_clockwrlock.001
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <time.h>

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_rwlock_t l = PTHREAD_RWLOCK_INITIALIZER;
	struct timespec past = { 0, 0 };
	int r, k;

	pc_begin("XSH.pthread_rwlock_clockwrlock.001");
	pc_test();
	for (k = 0; k < 3; k++) {
		const char *what = k == 0 ? "pthread_rwlock_timedwrlock()" : k == 1 ? "pthread_rwlock_clockwrlock(CLOCK_REALTIME)" : "pthread_rwlock_clockwrlock(CLOCK_MONOTONIC)";

		if (k == 0)
			r = pthread_rwlock_timedwrlock(&l, &past);
		else
			r = pthread_rwlock_clockwrlock(&l, k == 1 ? CLOCK_REALTIME : CLOCK_MONOTONIC, &past);
		pc_check(r == 0, "%s of an unlocked lock with a past time returned %s", what, rn(r));
		r = pthread_rwlock_unlock(&l);
		pc_check(r == 0, "pthread_rwlock_unlock() returned %s", rn(r));
	}
	pc_pass();
}
