/*
 * A thread created by a thread in the default floating-point state starts in it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.threads.006
 */
#include "../../../../check.h"
#include <pthread.h>
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.threads.006")
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
	pc_begin("ABI.i386.threads.006");
	abi_require_sse();
	pc_check((abi_x87_cw() & 0xFFFFu) == ABI_X87_CW_DEFAULT && (abi_mxcsr() & ABI_MXCSR_CONTROL) == (ABI_MXCSR_DEFAULT & ABI_MXCSR_CONTROL),
	    "setup: the creating thread is not in the default floating-point state (x87 0x%04x, MXCSR 0x%08x)", abi_x87_cw(), abi_mxcsr());
	pc_test();
	run_thread(NULL);
	pc_check(thr_tramp_rec.cw == ABI_X87_CW_DEFAULT, "the new thread started with x87 control word 0x%04x, expected 0x%04x",
	    thr_tramp_rec.cw, ABI_X87_CW_DEFAULT);
	pc_check((thr_tramp_rec.mxcsr & ABI_MXCSR_CONTROL) == (ABI_MXCSR_DEFAULT & ABI_MXCSR_CONTROL),
	    "the new thread started with MXCSR control bits 0x%04x, expected 0x%04x",
	    thr_tramp_rec.mxcsr & ABI_MXCSR_CONTROL, ABI_MXCSR_DEFAULT & ABI_MXCSR_CONTROL);
	pc_pass();
}
#endif
