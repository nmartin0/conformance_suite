/*
 * For children that call _exit() with 0, 1, 2, 63, 64, 127, 128, 129, 254
 * and 255, WIFEXITED is true, WEXITSTATUS gives the value, and
 * WIFSIGNALED, WIFSTOPPED and (when defined) WIFCONTINUED are false.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_wait.h.005
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t fired;

static void on_alarm(int sig)
{
	(void)sig;
	fired = 1;
}

int main(void)
{
	static const int codes[] = { 0, 1, 2, 63, 64, 127, 128, 129, 254, 255 };
	struct sigaction sa;
	unsigned k;

	pc_begin("XBD.sys_wait.h.005");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(60);
	pc_test();
	for (k = 0; k < sizeof codes / sizeof codes[0]; k++) {
		pid_t c = fork();
		int st = 0;

		if (c == -1)
			pc_unresolved("fork failed");
		if (c == 0)
			_exit(codes[k]);
		pc_check(waitpid(c, &st, 0) == c, "waitpid failed (%s)", pc_errname(errno));
		pc_check(WIFEXITED(st), "WIFEXITED is false for a child that called _exit(%d) (status %#x)", codes[k], (unsigned)st);
		pc_check(WEXITSTATUS(st) == codes[k], "WEXITSTATUS is %d for a child that called _exit(%d)", WEXITSTATUS(st), codes[k]);
		pc_check(!WIFSIGNALED(st) && !WIFSTOPPED(st), "WIFSIGNALED or WIFSTOPPED is true for a child that called _exit(%d)", codes[k]);
#ifdef WIFCONTINUED
		pc_check(!WIFCONTINUED(st), "WIFCONTINUED is true for a child that called _exit(%d)", codes[k]);
#endif
	}
	pc_check(!fired, "the guard alarm fired");
	pc_pass();
}
