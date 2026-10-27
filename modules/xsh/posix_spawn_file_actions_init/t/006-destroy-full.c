/*
 * posix_spawn_file_actions_destroy() on an object holding close, open and dup2 actions...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawn_file_actions_init.006
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
	pc_begin("XSH.posix_spawn_file_actions_init.006");

	posix_spawn_file_actions_t fa;
	int fd, fd2, r;

	pc_make_file("keep", "k");
	fd = open("keep", O_RDONLY);
	pc_check(fd != -1, "cannot open the test file");
	fd2 = open("keep", O_RDONLY);
	pc_check(fd2 != -1, "cannot open the test file");
	(void)close(fd2);
	pc_check(posix_spawn_file_actions_init(&fa) == 0, "init failed");
	pc_check(posix_spawn_file_actions_addclose(&fa, fd) == 0, "addclose failed");
	pc_check(posix_spawn_file_actions_addopen(&fa, fd2, "keep", O_RDONLY, 0) == 0, "addopen failed");
	pc_check(posix_spawn_file_actions_adddup2(&fa, fd, fd2) == 0, "adddup2 failed");
	pc_test();
	r = posix_spawn_file_actions_destroy(&fa);
	pc_check(r == 0, "posix_spawn_file_actions_destroy() returned %s", pc_errname(r));
	pc_pass();
}
