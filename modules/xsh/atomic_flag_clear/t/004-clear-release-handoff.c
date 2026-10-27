/*
 * Ordinary data written before atomic_flag_clear_explicit(release) is
 * visible to the thread whose atomic_flag_test_and_set_explicit(acquire)
 * then returns false.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_flag_clear.004
 */
#define PCAT_ID "XSH.atomic_flag_clear.004"
#include "pcat.h"
#ifndef PCAT_NONE

#define N 2000

static atomic_flag token = ATOMIC_FLAG_INIT;
static int data[N + 1];		/* ordinary memory, one cell per round */
static atomic_int taken;	/* rounds the consumer has read */
static atomic_int bad, timedout;

static void *thread_main(void *arg)
{
	int idx = (int)(uintptr_t)arg;
	int k;
	unsigned n = 0;
	long long t0;

	pcat_wait_go();
	t0 = pcat_ms();
	for (k = 1; k <= N; k++) {
		if (idx == 0) {
			/* producer: wait for the consumer to be done with the last round */
			while (atomic_load_explicit(&taken, memory_order_acquire) != k - 1) {
				pcat_relax(&n);
				if (pcat_expired(t0, 20000)) {
					atomic_store(&timedout, 1);
					atomic_store(&pcat_abort, 1);
					return NULL;
				}
			}
			data[k] = k * 5 + 3;
			atomic_flag_clear_explicit(&token, memory_order_release);
		} else {
			/* consumer: the flag reads false once the producer cleared it */
			while (atomic_flag_test_and_set_explicit(&token, memory_order_acquire)) {
				pcat_relax(&n);
				if (pcat_expired(t0, 20000)) {
					atomic_store(&timedout, 1);
					atomic_store(&pcat_abort, 1);
					return NULL;
				}
			}
			if (data[k] != k * 5 + 3)
				atomic_fetch_add(&bad, 1);
			atomic_store_explicit(&taken, k, memory_order_release);
		}
	}
	return NULL;
}

int main(void)
{
	pc_begin(PCAT_ID);
	/* the consumer starts out holding the token: the flag is set */
	pc_check(!atomic_flag_test_and_set(&token), "a flag from ATOMIC_FLAG_INIT was not clear");
	pc_test();
	if (pcat_par(2, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	pc_check(atomic_load(&bad) == 0, "data written before the clear was not visible after the flag read false (%d rounds)",
	    atomic_load(&bad));
	if (atomic_load(&timedout))
		pc_unresolved("the hand-off stalled for 20 s");
	pc_pass();
}
#endif
