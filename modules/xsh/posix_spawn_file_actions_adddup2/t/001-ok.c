/*
 * posix_spawn_file_actions_adddup2() with an open descriptor and a different number...
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_spawn_file_actions_adddup2.001
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
	pc_begin("XSH.posix_spawn_file_actions_adddup2.001");

	posix_spawn_file_actions_t fa;
	int a, b, r;

	pc_make_file("data", "0123456789");
	a = open("data", O_RDONLY);
	pc_check(a != -1, "cannot open the test file");
	b = free_fd_plain();
	pc_check(posix_spawn_file_actions_init(&fa) == 0, "init failed");
	pc_test();
	r = posix_spawn_file_actions_adddup2(&fa, a, b);
	pc_check(r == 0, "posix_spawn_file_actions_adddup2() returned %s", pc_errname(r));
	(void)posix_spawn_file_actions_destroy(&fa);
	pc_pass();
}
