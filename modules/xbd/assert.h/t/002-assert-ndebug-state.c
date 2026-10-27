/*
 * A failing assert aborts, a disabled one does not.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.assert.h.002
 */
#undef NDEBUG
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include "pc.h"

static void active_fail(void)
{
	assert(0);
}

#define NDEBUG
#include <assert.h>

static void disabled_fail(void)
{
	assert(0);
}

/* Runs f in a child with no core file and no messages; returns its wait status. */
static int child(void (*f)(void))
{
	pid_t p = fork();
	int st;

	if (p < 0)
		pc_unresolved("fork failed");
	if (p == 0) {
		struct rlimit rl = { 0, 0 };
		int fd = open("/dev/null", O_WRONLY);

		setrlimit(RLIMIT_CORE, &rl);
		if (fd >= 0)
			dup2(fd, 2);
		f();
		_Exit(0);
	}
	if (waitpid(p, &st, 0) != p)
		pc_unresolved("waitpid failed");
	return st;
}

int main(void)
{
	pc_begin("XBD.assert.h.002");
	pc_test();
	int st;

	st = child(disabled_fail);
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "a disabled assert(0) did not let the process carry on (status %d)", st);
	st = child(active_fail);
	pc_check(!(WIFEXITED(st) && WEXITSTATUS(st) == 0), "assert(0) with NDEBUG undefined let the process carry on");
	pc_pass();
}
