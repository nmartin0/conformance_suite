/*
 * posix_spawnattr_setflags() with the bitwise OR of POSIX_SPAWN_RESETIDS,...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getflags.003
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
	pc_begin("XSH.posix_spawnattr_getflags.003");

	posix_spawnattr_t at;
	short want = POSIX_SPAWN_RESETIDS | POSIX_SPAWN_SETSIGDEF | POSIX_SPAWN_SETSIGMASK, got = 0;
	int r;

	pc_check(posix_spawnattr_init(&at) == 0, "posix_spawnattr_init failed");
	pc_test();
	r = posix_spawnattr_setflags(&at, want);
	pc_check(r == 0, "setflags() returned %s", pc_errname(r));
	r = posix_spawnattr_getflags(&at, &got);
	pc_check(r == 0, "getflags() returned %s", pc_errname(r));
	pc_check(got == want, "the flags are %#x, expected %#x", got, want);
	(void)posix_spawnattr_destroy(&at);
	pc_pass();
}
