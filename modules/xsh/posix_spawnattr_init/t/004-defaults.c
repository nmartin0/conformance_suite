/*
 * After posix_spawnattr_init() on an arbitrary-bytes object, the spawn-flags attribute...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_init.004
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <spawn.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* A descriptor number that is in range and not open. */
static int free_fd_plain(void)
{
	int f = open("/dev/null", O_RDONLY);

	if (f == -1)
		pc_unresolved("cannot open /dev/null");
	(void)close(f);
	return f;
}

int main(void)
{
	pc_begin("XSH.posix_spawnattr_init.004");

	posix_spawnattr_t at;
	sigset_t s;
	short fl = 0x55;
	pid_t pg = 12345;
	int r, i;
	const int sigs[] = { SIGHUP, SIGINT, SIGQUIT, SIGABRT, SIGALRM, SIGTERM, SIGUSR1, SIGUSR2, SIGCHLD,
	    SIGCONT, SIGPIPE, SIGTSTP, SIGTTIN, SIGTTOU, SIGBUS, SIGFPE, SIGILL, SIGSEGV };

	memset(&at, 0xA5, sizeof at);
	sigfillset(&s);
	pc_test();
	r = posix_spawnattr_init(&at);
	pc_check(r == 0, "posix_spawnattr_init() returned %s", pc_errname(r));
	r = posix_spawnattr_getflags(&at, &fl);
	pc_check(r == 0 && fl == 0, "getflags returned %s and stored %#x, expected 0 and 0", pc_errname(r), fl);
	r = posix_spawnattr_getpgroup(&at, &pg);
	pc_check(r == 0 && pg == 0, "getpgroup returned %s and stored %ld, expected 0 and 0", pc_errname(r), (long)pg);
	r = posix_spawnattr_getsigdefault(&at, &s);
	pc_check(r == 0, "getsigdefault returned %s", pc_errname(r));
	for (i = 0; i < (int)(sizeof sigs / sizeof sigs[0]); i++)
		pc_check(sigismember(&s, sigs[i]) == 0, "signal %d is in the default sigdefault set", sigs[i]);
	(void)posix_spawnattr_destroy(&at);
	pc_pass();
}
