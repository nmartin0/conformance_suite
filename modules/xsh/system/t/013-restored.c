/*
 * SIGINT and SIGQUIT actions are restored afterwards.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.013
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static void on_sig(int sig)
{
	(void)sig;
}

static void one(int sig, int kind)
{
	struct sigaction sa, old;
	int r;

	memset(&sa, 0, sizeof sa);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = kind == 0 ? on_sig : kind == 1 ? SIG_IGN : SIG_DFL;
	pc_check(sigaction(sig, &sa, NULL) == 0, "sigaction failed");
	r = system("exit 0");
	pc_check(r != -1 && WIFEXITED(r) && WEXITSTATUS(r) == 0, "system() gave %d", r);
	memset(&old, 0xff, sizeof old);
	pc_check(sigaction(sig, NULL, &old) == 0, "sigaction failed");
	pc_check(!(old.sa_flags & SA_SIGINFO) || kind != 0, "the handler's SA_SIGINFO flag changed");
	pc_check(old.sa_handler == sa.sa_handler, "after system(), the action for signal %d is not what it was (kind %d)", sig, kind);
}

int main(void)
{
	int kind;

	pc_begin("XSH.system.013");
	pc_test();
	for (kind = 0; kind < 3; kind++) {
		one(SIGINT, kind);
		one(SIGQUIT, kind);
	}
	pc_pass();
}
