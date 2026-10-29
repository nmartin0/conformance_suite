/*
 * A thread's start routine is entered with the direction flag clear.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.threads.003
 */
#include "../../../../check.h"
#include <pthread.h>
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.threads.003")
#else
ABI_TARGET void *thr_body(void *arg)
{
	if (arg != NULL) {      /* a thread that changes its own floating-point state */
		abi_x87_set_cw(ABI_X87_CW_ALT);
		abi_set_mxcsr(ABI_MXCSR_ALT);
	}
	return arg;
}

void *thr_tramp(void *arg);
ABI_ENTRY_TRAMPOLINE(thr_tramp, thr_body);

/* Start one thread at the recording trampoline and wait for it. */
static void run_thread(void *arg)
{
	pthread_t t;

	if (pthread_create(&t, NULL, thr_tramp, arg) != 0)
		pc_unresolved("pthread_create failed");
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join failed");
}
int main(void)
{
	pc_begin("ABI.i386.threads.003");
	pc_test();
	run_thread(NULL);
	abi_expect_entry_df_clear(&thr_tramp_rec, "the thread start routine");
	pc_pass();
}
#endif
