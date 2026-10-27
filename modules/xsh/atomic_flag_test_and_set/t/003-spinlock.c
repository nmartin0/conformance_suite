/*
 * A spin lock made of atomic_flag_test_and_set() (acquire) and
 * atomic_flag_clear() (release) gives mutual exclusion: four threads
 * increment an ordinary counter under it and no increment is lost or
 * overlapped.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_flag_test_and_set.003
 */
#define PCAT_ID "XSH.atomic_flag_test_and_set.003"
#include "pcat.h"
#ifndef PCAT_NONE

#define T 4
#define M 5000

static atomic_flag lock = ATOMIC_FLAG_INIT;
static long counter;		/* ordinary memory, protected by lock */
static volatile int inside;	/* ordinary memory, protected by lock */
static int overlap;		/* ordinary memory, protected by lock */
static atomic_int gaveup;

static void *thread_main(void *arg)
{
	int idx = (int)(uintptr_t)arg;
	int j;

	pcat_wait_go();
	for (j = 0; j < M; j++) {
		unsigned n = 0;
		long long t0 = pcat_ms();

		if (idx & 1) {
			while (atomic_flag_test_and_set(&lock)) {
				pcat_relax(&n);
				if ((n & 0xfffu) == 0 && pcat_expired(t0, 30000)) {
					atomic_store(&gaveup, 1);
					return NULL;
				}
			}
		} else {
			while (atomic_flag_test_and_set_explicit(&lock, memory_order_acquire)) {
				pcat_relax(&n);
				if ((n & 0xfffu) == 0 && pcat_expired(t0, 30000)) {
					atomic_store(&gaveup, 1);
					return NULL;
				}
			}
		}
		if (inside)
			overlap++;
		inside = 1;
		counter++;
		inside = 0;
		if (idx & 1)
			atomic_flag_clear(&lock);
		else
			atomic_flag_clear_explicit(&lock, memory_order_release);
	}
	return NULL;
}

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
	if (pcat_par(T, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	if (atomic_load(&gaveup))
		pc_unresolved("a thread could not take the lock within 30 s");
	pc_check(overlap == 0, "two threads were inside the lock at once (%d times)", overlap);
	pc_check(counter == (long)T * M, "counter is %ld after %d locked increments", counter, T * M);
	pc_pass();
}
#endif
