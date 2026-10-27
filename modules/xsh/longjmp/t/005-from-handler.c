/*
 * longjmp() from a signal handler.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.longjmp.005
 */
#include "pc.h"
#include <setjmp.h>
#include <signal.h>
#include <string.h>

static jmp_buf env;

static void handler(int sig)
{
	(void)sig;
	longjmp(env, 5);
}

int main(void)
{
	struct sigaction sa;
	volatile int direct = 0;
	sigset_t all;

	pc_begin("XSH.longjmp.005");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed");
	pc_test();
	switch (setjmp(env)) {
	case 0:
		direct++;
		(void)raise(SIGUSR1);
		pc_fail("raise() returned without the handler jumping away");
	case 5:
		break;
	default:
		pc_fail("setjmp() returned another value than 5");
	}
	/* Whether the mask was restored is unspecified: make sure nothing stays blocked. */
	sigemptyset(&all);
	(void)sigprocmask(SIG_SETMASK, &all, NULL);
	pc_check(direct == 1, "the direct path ran %d times", direct);
	pc_pass();
}
