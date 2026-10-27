/*
 * The command starts with fork and exec's signal handling.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.014
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
#include <sys/resource.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static void on_sig(int sig)
{
	(void)sig;
}

static void set_both(void (*h)(int))
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = h;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGINT, &sa, NULL) == 0 && sigaction(SIGQUIT, &sa, NULL) == 0, "sigaction failed");
}

int main(void)
{
	struct rlimit rl;
	int r;

	pc_begin("XSH.system.014");
	/* SIGQUIT may leave a core image; ask for none (lowering the limit is always allowed). */
	pc_check(getrlimit(RLIMIT_CORE, &rl) == 0, "getrlimit failed");
	rl.rlim_cur = 0;
	pc_check(setrlimit(RLIMIT_CORE, &rl) == 0, "setrlimit failed");
	pc_test();
	set_both(on_sig);
	r = system("kill -s INT $$; exit 5");
	pc_check(r != -1, "system() failed: %s", pc_errname(errno));
	/* A shell may take its own default-action signal and leave with a status above 128 instead of dying of it. */
	pc_check((WIFSIGNALED(r) && WTERMSIG(r) == SIGINT) || (WIFEXITED(r) && WEXITSTATUS(r) > 128),
	    "with SIGINT caught in the caller, the command's SIGINT to itself gave status %#x, expected death by SIGINT (default action)", r);
	r = system("kill -s QUIT $$; exit 5");
	pc_check(r != -1, "system() failed: %s", pc_errname(errno));
	pc_check((WIFSIGNALED(r) && WTERMSIG(r) == SIGQUIT) || (WIFEXITED(r) && WEXITSTATUS(r) > 128),
	    "with SIGQUIT caught in the caller, the command's SIGQUIT to itself gave status %#x, expected death by SIGQUIT (default action)", r);
	set_both(SIG_IGN);
	r = system("kill -s INT $$; kill -s QUIT $$; exit 5");
	pc_check(r != -1, "system() failed: %s", pc_errname(errno));
	pc_check(WIFEXITED(r) && WEXITSTATUS(r) == 5,
	    "with SIGINT and SIGQUIT ignored in the caller, the command gave status %#x, expected exit 5 (they stay ignored)", r);
	pc_pass();
}
