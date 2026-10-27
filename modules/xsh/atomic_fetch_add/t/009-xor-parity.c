/*
 * Concurrent atomic_fetch_xor() calls lose no update: with three threads
 * flipping a shared bit and each one a private bit an odd number of
 * times, the final value is exactly the expected parity pattern.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.009
 */
#define PCAT_ID "XSH.atomic_fetch_add.009"
#include "pcat.h"
#ifndef PCAT_NONE

#define T 3
#define M 10001

static atomic_uint v;
static atomic_uint wrong;

static void *thread_main(void *arg)
{
	unsigned idx = (unsigned)(uintptr_t)arg;
	unsigned own = 2u << idx;
	unsigned old;
	int j;

	pcat_wait_go();
	for (j = 0; j < M; j++) {
		if (idx & 1)
			atomic_fetch_xor_explicit(&v, 1u, memory_order_seq_cst);
		else
			atomic_fetch_xor(&v, 1u);
		old = atomic_fetch_xor(&v, own);
		/* only this thread flips its bit, so it alternates 0, 1, 0, ... */
		if (((old & own) != 0) != (j & 1))
			atomic_fetch_add(&wrong, 1);
	}
	return NULL;
}

int main(void)
{
	unsigned fin;

	pc_begin(PCAT_ID);
	atomic_store(&v, 0);
	pc_test();
	if (pcat_par(T, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	fin = atomic_load(&v);
	pc_check(atomic_load(&wrong) == 0, "a private bit was not as this thread left it (%u times)",
	    atomic_load(&wrong));
	/* 3 * 10001 flips of bit 0 (odd) and 10001 flips (odd) of each of bits 1 to 3 */
	pc_check(fin == 0xfu, "final value is %#x, expected 0xf", fin);
	pc_pass();
}
#endif
