/*
 * sigsetjmp() with savemask saves the signal mask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigsetjmp.003
 */
#include "pc.h"
#include <setjmp.h>
#include <signal.h>

static sigjmp_buf env;

int main(void)
{
	sigset_t base, changed, now;
	volatile int jumped = 0;

	pc_begin("XSH.sigsetjmp.003");
	sigemptyset(&base);
	sigaddset(&base, SIGUSR2);
	pc_check(sigprocmask(SIG_SETMASK, &base, NULL) == 0, "sigprocmask failed");
	sigemptyset(&changed);
	sigaddset(&changed, SIGUSR1);
	pc_test();
	if (sigsetjmp(env, 1) == 0) {
		/* Block SIGUSR1 and unblock SIGUSR2, then jump. */
		pc_check(sigprocmask(SIG_SETMASK, &changed, NULL) == 0, "sigprocmask failed");
		siglongjmp(env, 1);
	}
	jumped++;
	pc_check(sigprocmask(SIG_BLOCK, NULL, &now) == 0, "sigprocmask failed");
	pc_check(sigismember(&now, SIGUSR2) == 1, "SIGUSR2, blocked at the time of sigsetjmp(), is not blocked after the jump");
	pc_check(sigismember(&now, SIGUSR1) == 0, "SIGUSR1, unblocked at the time of sigsetjmp(), is blocked after the jump");
	pc_check(jumped == 1, "the jump happened %d times", jumped);
	pc_pass();
}
