/*
 * A test starts with SIGPIPE and SIGINT at their default action and no
 * signal blocked, whatever the driver inherited.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ST.c.016
 */
#include "pc.h"
#include <signal.h>

int main(void)
{
	struct sigaction a, b;
	sigset_t m;

	pc_begin("ST.c.016");
	pc_test();
	pc_check(sigaction(SIGPIPE, NULL, &a) == 0 && sigaction(SIGINT, NULL, &b) == 0, "sigaction failed");
	pc_check(a.sa_handler == SIG_DFL && b.sa_handler == SIG_DFL, "a signal is not at its default action");
	pc_check(sigprocmask(SIG_SETMASK, NULL, &m) == 0, "sigprocmask failed");
	pc_check(!sigismember(&m, SIGPIPE) && !sigismember(&m, SIGALRM), "a signal is blocked");
	pc_pass();
}
