/*
 * A thread's changes to its floating-point control state do not reach its creator or a later thread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.threads.007
 */
#include "../../../../check.h"
#include <pthread.h>
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.threads.007")
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
	pc_begin("ABI.i386.threads.007");
	unsigned cw, mx;

	abi_require_sse();
	cw = abi_x87_cw();
	mx = abi_mxcsr();
	pc_test();
	run_thread(&cw);                /* this thread sets round-down in its own state */
	pc_check(abi_x87_cw() == cw, "x87 control word 0x%04x in the creator after a thread changed its own to 0x%04x", abi_x87_cw(), ABI_X87_CW_ALT);
	pc_check(((abi_mxcsr() ^ mx) & ABI_MXCSR_CONTROL) == 0, "MXCSR control bits changed in the creator after a thread changed its own");
	run_thread(NULL);               /* a thread started afterwards inherits the creator's, not that one's */
	pc_check(thr_tramp_rec.cw == cw, "a thread started later had x87 control word 0x%04x, expected the creator's 0x%04x", thr_tramp_rec.cw, cw);
	pc_check(((thr_tramp_rec.mxcsr ^ mx) & ABI_MXCSR_CONTROL) == 0, "a thread started later had MXCSR control bits 0x%04x, expected the creator's 0x%04x",
	    thr_tramp_rec.mxcsr & ABI_MXCSR_CONTROL, mx & ABI_MXCSR_CONTROL);
	pc_pass();
}
#endif
