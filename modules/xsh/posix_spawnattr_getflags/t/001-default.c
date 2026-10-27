/*
 * posix_spawnattr_getflags() on a newly initialised object returns 0 and stores 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getflags.001
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
	pc_begin("XSH.posix_spawnattr_getflags.001");

	posix_spawnattr_t at;
	short fl = 0x5a;
	int r;

	pc_check(posix_spawnattr_init(&at) == 0, "posix_spawnattr_init failed");
	pc_test();
	r = posix_spawnattr_getflags(&at, &fl);
	pc_check(r == 0, "posix_spawnattr_getflags() returned %s", pc_errname(r));
	pc_check(fl == 0, "the default flags are %#x, expected 0", fl);
	(void)posix_spawnattr_destroy(&at);
	pc_pass();
}
