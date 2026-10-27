/*
 * siglongjmp() restores the saved mask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.siglongjmp.003
 */
#include "pc.h"
#include <setjmp.h>
#include <signal.h>

static sigjmp_buf env;

int main(void)
{
	sigset_t base, changed, now;

	pc_begin("XSH.siglongjmp.003");
	sigemptyset(&base);
	sigaddset(&base, SIGUSR2);
	pc_check(sigprocmask(SIG_SETMASK, &base, NULL) == 0, "sigprocmask failed");
	sigemptyset(&changed);
	sigaddset(&changed, SIGUSR1);
	pc_test();
	if (sigsetjmp(env, 1) == 0) {
		pc_check(sigprocmask(SIG_SETMASK, &changed, NULL) == 0, "sigprocmask failed");
		siglongjmp(env, 1);
	}
	pc_check(sigprocmask(SIG_BLOCK, NULL, &now) == 0, "sigprocmask failed");
	pc_check(sigismember(&now, SIGUSR2) == 1, "SIGUSR2 was blocked at sigsetjmp() but is not after the jump");
	pc_check(sigismember(&now, SIGUSR1) == 0, "SIGUSR1 was unblocked at sigsetjmp() but is blocked after the jump");
	pc_pass();
}
