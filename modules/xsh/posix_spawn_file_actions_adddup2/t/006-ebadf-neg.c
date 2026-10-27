/*
 * posix_spawn_file_actions_adddup2() with a negative fildes, and with a negative...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawn_file_actions_adddup2.006
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
	pc_begin("XSH.posix_spawn_file_actions_adddup2.006");

	posix_spawn_file_actions_t fa;
	int a, r[4];

	pc_make_file("data", "x");
	a = open("data", O_RDONLY);
	pc_check(a != -1, "cannot open the test file");
	pc_check(posix_spawn_file_actions_init(&fa) == 0, "init failed");
	pc_test();
	r[0] = posix_spawn_file_actions_adddup2(&fa, -1, a);
	r[1] = posix_spawn_file_actions_adddup2(&fa, INT_MIN, a);
	r[2] = posix_spawn_file_actions_adddup2(&fa, a, -1);
	r[3] = posix_spawn_file_actions_adddup2(&fa, a, INT_MIN);
	pc_check(r[0] == EBADF, "adddup2(-1, fd) returned %s, expected EBADF", pc_errname(r[0]));
	pc_check(r[1] == EBADF, "adddup2(INT_MIN, fd) returned %s, expected EBADF", pc_errname(r[1]));
	pc_check(r[2] == EBADF, "adddup2(fd, -1) returned %s, expected EBADF", pc_errname(r[2]));
	pc_check(r[3] == EBADF, "adddup2(fd, INT_MIN) returned %s, expected EBADF", pc_errname(r[3]));
	(void)posix_spawn_file_actions_destroy(&fa);
	pc_pass();
}
