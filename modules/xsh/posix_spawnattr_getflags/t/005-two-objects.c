/*
 * Two initialised objects keep their own flags: different values set in each are read...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getflags.005
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
	pc_begin("XSH.posix_spawnattr_getflags.005");

	posix_spawnattr_t a, b;
	short ga = 0, gb = 0;

	pc_check(posix_spawnattr_init(&a) == 0 && posix_spawnattr_init(&b) == 0, "posix_spawnattr_init failed");
	pc_test();
	pc_check(posix_spawnattr_setflags(&a, POSIX_SPAWN_SETSIGDEF) == 0, "setflags failed");
	pc_check(posix_spawnattr_setflags(&b, POSIX_SPAWN_SETSIGMASK) == 0, "setflags failed");
	pc_check(posix_spawnattr_getflags(&a, &ga) == 0 && posix_spawnattr_getflags(&b, &gb) == 0, "getflags failed");
	pc_check(ga == POSIX_SPAWN_SETSIGDEF, "the first object's flags are %#x", ga);
	pc_check(gb == POSIX_SPAWN_SETSIGMASK, "the second object's flags are %#x", gb);
	(void)posix_spawnattr_destroy(&a);
	(void)posix_spawnattr_destroy(&b);
	pc_pass();
}
