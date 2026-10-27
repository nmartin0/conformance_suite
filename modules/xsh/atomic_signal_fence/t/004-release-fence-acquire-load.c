/*
 * Data written before a release fence followed by a relaxed store is
 * visible to a thread that reads that value with an acquire load.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_signal_fence.004
 */
#define PCAT_ID "XSH.atomic_signal_fence.004"
#include "pcat.h"
#ifndef PCAT_NONE

#define N 20000

static int data[N + 1];		/* ordinary memory, one cell per message */
static atomic_int seq;
static atomic_int bad, reached;

static void *thread_main(void *arg)
{
	int idx = (int)(uintptr_t)arg;
	int k;

	pcat_wait_go();
	if (idx == 0) {
		for (k = 1; k <= N; k++) {
			data[k] = k * 3 + 1;
			atomic_thread_fence(memory_order_release);
			atomic_store_explicit(&seq, k, memory_order_relaxed);
		}
	} else {
		int last = 0, s;
		long long t0 = pcat_ms();
		unsigned n = 0;

		while (last < N && !pcat_expired(t0, 20000)) {
			s = atomic_load_explicit(&seq, memory_order_acquire);
			for (k = last + 1; k <= s; k++)
				if (data[k] != k * 3 + 1)
					atomic_fetch_add(&bad, 1);
			if (s > last)
				last = s;
			pcat_relax(&n);
		}
		if (last == N)
			atomic_store(&reached, 1);
	}
	return NULL;
}

int main(void)
{
	pc_begin(PCAT_ID);
	atomic_store(&seq, 0);
	pc_test();
	if (pcat_par(2, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	pc_check(atomic_load(&bad) == 0, "data written before the publishing step was not visible to the reader (%d cells)",
	    atomic_load(&bad));
	if (!atomic_load(&reached))
		pc_unresolved("the last stored value was not seen within 20 s");
	pc_pass();
}
#endif
