/*
 * posix_spawn_file_actions_adddup2() with fildes equal to {OPEN_MAX}, and with...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawn_file_actions_adddup2.007
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
	pc_begin("XSH.posix_spawn_file_actions_adddup2.007");

	posix_spawn_file_actions_t fa;
	long om = sysconf(_SC_OPEN_MAX);
	int a, r1, r2;

	if (om < 1 || om > INT_MAX)
		pc_unresolved("{OPEN_MAX} is indeterminate or too large for int");
	pc_make_file("data", "x");
	a = open("data", O_RDONLY);
	pc_check(a != -1 && a < om, "cannot open the test file below the limit");
	pc_check(posix_spawn_file_actions_init(&fa) == 0, "init failed");
	pc_test();
	r1 = posix_spawn_file_actions_adddup2(&fa, (int)om, a);
	r2 = posix_spawn_file_actions_adddup2(&fa, a, (int)om);
	pc_check(r1 == EBADF, "adddup2(OPEN_MAX, fd) returned %s, expected EBADF", pc_errname(r1));
	pc_check(r2 == EBADF, "adddup2(fd, OPEN_MAX) returned %s, expected EBADF", pc_errname(r2));
	(void)posix_spawn_file_actions_destroy(&fa);
	pc_pass();
}
