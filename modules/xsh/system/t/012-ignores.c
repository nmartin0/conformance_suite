/*
 * SIGINT and SIGQUIT are ignored in the caller during system().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.012
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

static volatile sig_atomic_t hits;

static void on_sig(int sig)
{
	(void)sig;
	hits++;
}

int main(void)
{
	struct sigaction sa;
	int r;

	pc_begin("XSH.system.012");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_sig;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGINT, &sa, NULL) == 0 && sigaction(SIGQUIT, &sa, NULL) == 0, "sigaction failed");
	pc_test();
	r = system("kill -s INT $PPID; kill -s QUIT $PPID; exit 0");
	pc_check(r != -1, "system() failed: %s", pc_errname(errno));
	pc_check(WIFEXITED(r) && WEXITSTATUS(r) == 0, "status %#x", r);
	pc_check(hits == 0, "the caller's handlers ran %d time(s) for signals sent while system() waited", (int)hits);
	pc_pass();
}
