/*
 * posix_spawn_file_actions_addfchdir() with a negative descriptor (-1, and the most...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawn_file_actions_addchdir.009
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
	pc_begin("XSH.posix_spawn_file_actions_addchdir.009");

	posix_spawn_file_actions_t fa;
	int r1, r2;

	pc_check(posix_spawn_file_actions_init(&fa) == 0, "init failed");
	pc_test();
	r1 = posix_spawn_file_actions_addfchdir(&fa, -1);
	r2 = posix_spawn_file_actions_addfchdir(&fa, INT_MIN);
	pc_check(r1 == EBADF, "addfchdir(-1) returned %s, expected EBADF", pc_errname(r1));
	pc_check(r2 == EBADF, "addfchdir(INT_MIN) returned %s, expected EBADF", pc_errname(r2));
	(void)posix_spawn_file_actions_destroy(&fa);
	pc_pass();
}
