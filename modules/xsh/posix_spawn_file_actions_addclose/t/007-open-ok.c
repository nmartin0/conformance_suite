/*
 * posix_spawn_file_actions_addopen() with a valid descriptor number, a pathname, flags...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawn_file_actions_addclose.007
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
	pc_begin("XSH.posix_spawn_file_actions_addclose.007");

	posix_spawn_file_actions_t fa;
	int fd, r;

	pc_make_file("keep", "k");
	fd = free_fd_plain();
	pc_check(posix_spawn_file_actions_init(&fa) == 0, "init failed");
	pc_test();
	r = posix_spawn_file_actions_addopen(&fa, fd, "keep", O_RDONLY, 0);
	pc_check(r == 0, "posix_spawn_file_actions_addopen() returned %s", pc_errname(r));
	(void)posix_spawn_file_actions_destroy(&fa);
	pc_pass();
}
