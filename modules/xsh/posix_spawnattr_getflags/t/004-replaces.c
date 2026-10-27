/*
 * A later posix_spawnattr_setflags() replaces the earlier value: after setting a flag...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getflags.004
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
	pc_begin("XSH.posix_spawnattr_getflags.004");

	posix_spawnattr_t at;
	short got = 0x5a;

	pc_check(posix_spawnattr_init(&at) == 0, "posix_spawnattr_init failed");
	pc_test();
	pc_check(posix_spawnattr_setflags(&at, POSIX_SPAWN_SETSIGDEF) == 0, "setflags failed");
	pc_check(posix_spawnattr_setflags(&at, 0) == 0, "setflags(0) failed");
	pc_check(posix_spawnattr_getflags(&at, &got) == 0 && got == 0, "after setflags(0) the flags are %#x", got);
	pc_check(posix_spawnattr_setflags(&at, POSIX_SPAWN_RESETIDS) == 0, "setflags failed");
	pc_check(posix_spawnattr_setflags(&at, POSIX_SPAWN_SETSIGMASK) == 0, "setflags failed");
	pc_check(posix_spawnattr_getflags(&at, &got) == 0 && got == POSIX_SPAWN_SETSIGMASK,
	    "after two settings the flags are %#x, expected only the second", got);
	(void)posix_spawnattr_destroy(&at);
	pc_pass();
}
