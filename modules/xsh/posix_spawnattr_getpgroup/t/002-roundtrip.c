/*
 * posix_spawnattr_setpgroup() with the caller's own process group ID returns 0 and...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawnattr_getpgroup.002
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
	pc_begin("XSH.posix_spawnattr_getpgroup.002");

	posix_spawnattr_t a, b;
	pid_t own = getpgrp(), g = 12345;
	int r;

	pc_check(posix_spawnattr_init(&a) == 0 && posix_spawnattr_init(&b) == 0, "posix_spawnattr_init failed");
	pc_test();
	r = posix_spawnattr_setpgroup(&a, own);
	pc_check(r == 0, "setpgroup(own group) returned %s", pc_errname(r));
	r = posix_spawnattr_getpgroup(&a, &g);
	pc_check(r == 0 && g == own, "getpgroup returned %s and stored %ld, expected %ld", pc_errname(r), (long)g, (long)own);
	r = posix_spawnattr_setpgroup(&b, 0);
	pc_check(r == 0, "setpgroup(0) returned %s", pc_errname(r));
	pc_check(posix_spawnattr_getpgroup(&a, &g) == 0 && g == own, "the first object's value changed to %ld", (long)g);
	pc_check(posix_spawnattr_getpgroup(&b, &g) == 0 && g == 0, "the second object's value is %ld, expected 0", (long)g);
	r = posix_spawnattr_setpgroup(&a, 0);
	pc_check(r == 0, "setpgroup(0) returned %s", pc_errname(r));
	pc_check(posix_spawnattr_getpgroup(&a, &g) == 0 && g == 0, "after setting 0 the value is %ld", (long)g);
	(void)posix_spawnattr_destroy(&a);
	(void)posix_spawnattr_destroy(&b);
	pc_pass();
}
