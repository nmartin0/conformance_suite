/*
 * siglongjmp() out of a signal handler.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.siglongjmp.005
 */
#include "pc.h"
#include <setjmp.h>
#include <signal.h>
#include <string.h>

static sigjmp_buf env;

static void handler(int sig)
{
	(void)sig;
	siglongjmp(env, 5);
}

int main(void)
{
	struct sigaction sa;
	sigset_t none, now;
	volatile int mask;

	pc_begin("XSH.siglongjmp.005");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed");
	sigemptyset(&none);
	pc_test();
	for (mask = 0; mask < 2; mask++) {
		pc_check(sigprocmask(SIG_SETMASK, &none, NULL) == 0, "sigprocmask failed");
		switch (sigsetjmp(env, mask)) {
		case 0:
			(void)raise(SIGUSR1);
			pc_fail("raise() returned without the handler jumping away");
		case 5:
			break;
		default:
			pc_fail("savemask %d: sigsetjmp() returned another value than 5", mask);
		}
		pc_check(sigprocmask(SIG_BLOCK, NULL, &now) == 0, "sigprocmask failed");
		if (mask)
			pc_check(sigismember(&now, SIGUSR1) == 0, "savemask 1: SIGUSR1 is still blocked after the jump");
		else
			pc_check(sigismember(&now, SIGUSR1) == 1, "savemask 0: SIGUSR1 is not blocked after the jump from its handler");
	}
	pc_check(sigprocmask(SIG_SETMASK, &none, NULL) == 0, "sigprocmask failed");
	pc_pass();
}
