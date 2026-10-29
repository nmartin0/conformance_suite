/*
 * sigtest.h - support for the signal checks.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A handler that records its entry state and then misbehaves on purpose
 * (changes registers and control words, sets DF) because the system
 * must put all of that back when the handler returns, and code that
 * is interrupted while sentinels sit in the callee-saved registers. Generic
 * C over the primitives of abi/arch/ARCH.h; included once per test.
 */
#ifndef ABI_SIGTEST_H
#define ABI_SIGTEST_H

#include "check.h"

#ifdef ABI_ARCH_I386

#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static volatile int sig_got;

ABI_TARGET void sig_hbody(int sig)
{
	sig_got = sig;
}

void sig_hclobber(int sig);
ABI_CLOBBERING_HANDLER(sig_hclobber, sig_hbody);

void sig_tramp(int sig);
ABI_ENTRY_TRAMPOLINE(sig_tramp, sig_hclobber);

static void sig_install(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = sig_tramp;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) != 0)
		pc_unresolved("sigaction(SIGUSR1) failed");
}

/* Delivery of a signal to the main thread once it is spinning, from another thread. */
struct sig_sender {
	struct abi_spin *s;
	pthread_t target;
};

static void *sig_sender_main(void *arg)
{
	struct sig_sender *x = arg;
	struct timespec ts = { 0, 1000000 };

	while (!x->s->ready)
		nanosleep(&ts, NULL);
	ts.tv_nsec = 30000000;
	nanosleep(&ts, NULL);       /* let the loop run for a while */
	pthread_kill(x->target, SIGUSR1);
	return NULL;
}

/*
 * Interrupt code that is spinning with sentinels in the callee-saved
 * registers, non-default control words, DF set if df, and ESP lowered by
 * misalign bytes; s records what it finds after the handler has run.
 */
static void sig_async(struct abi_spin *s, unsigned misalign, unsigned df)
{
	struct sig_sender x;
	pthread_t t;

	memset(s, 0, sizeof *s);
	s->in_reg[0] = ABI_SENT_EBX;
	s->in_reg[1] = ABI_SENT_ESI;
	s->in_reg[2] = ABI_SENT_EDI;
	s->in_reg[3] = ABI_SENT_EBP;
	s->cw_set = ABI_X87_CW_ALT;
	s->mxcsr_set = ABI_MXCSR_ALT;
	s->df = df;
	s->misalign = misalign;
	sig_got = 0;
	x.s = s;
	x.target = pthread_self();
	if (pthread_create(&t, NULL, sig_sender_main, &x) != 0)
		pc_unresolved("cannot create the thread that sends the signal");
	alarm(20);                  /* a signal that never arrives ends the test */
	abi_spin(&sig_got, s);
	alarm(0);
	pthread_join(t, NULL);
	if (sig_got != SIGUSR1)
		pc_unresolved("the handler was not run (flag %d)", sig_got);
	if (s->cw_before != ABI_X87_CW_ALT)
		pc_unresolved("could not load the x87 control word (it reads 0x%04x)", s->cw_before);
	if ((s->mxcsr_before & ABI_MXCSR_CONTROL) != (ABI_MXCSR_ALT & ABI_MXCSR_CONTROL))
		pc_unresolved("could not load MXCSR (it reads 0x%08x)", s->mxcsr_before);
}

#endif /* ABI_ARCH_I386 */
#endif /* ABI_SIGTEST_H */
