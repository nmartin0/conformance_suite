/*
 * Concurrent exchanges are atomic: threads swap unique tokens into one
 * object, and every token (and the initial value) is handed out exactly
 * once, as a returned value or as the final contents.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_exchange.004
 */
#define PCAT_ID "XSH.atomic_exchange.004"
#include "pcat.h"
#ifndef PCAT_NONE

#define T 4
#define M 5000

static atomic_int cell;
static atomic_int seen[T * M + 1];
static atomic_int outofrange;

static void *thread_main(void *arg)
{
	int idx = (int)(uintptr_t)arg;
	int j, tok, old;

	pcat_wait_go();
	for (j = 0; j < M; j++) {
		tok = idx * M + j + 1;
		if (idx & 1)
			old = atomic_exchange_explicit(&cell, tok, memory_order_acq_rel);
		else
			old = atomic_exchange(&cell, tok);
		if (old < 0 || old > T * M)
			atomic_fetch_add(&outofrange, 1);
		else
			atomic_fetch_add(&seen[old], 1);
	}
	return NULL;
}

int main(void)
{
	int k, fin;

	pc_begin(PCAT_ID);
	atomic_store(&cell, 0);
	pc_test();
	if (pcat_par(T, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	fin = atomic_load(&cell);
	pc_check(atomic_load(&outofrange) == 0, "an exchange returned a value nobody stored");
	pc_check(fin >= 0 && fin <= T * M, "the object ends with a value nobody stored");
	atomic_fetch_add(&seen[fin], 1);
	for (k = 0; k <= T * M; k++)
		pc_check(atomic_load(&seen[k]) == 1, "value %d was handed out %d times, not once", k,
		    atomic_load(&seen[k]));
	pc_pass();
}
#endif
