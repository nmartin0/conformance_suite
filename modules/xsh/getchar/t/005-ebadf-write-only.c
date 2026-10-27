/*
 * getchar() fails with EBADF when descriptor 0 is open for writing only.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getchar.005
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* Opens path with flags and makes it descriptor target (setup). */
static void fd_onto(const char *path, int flags, int target)
{
	int fd = open(path, flags, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	if (fd != target) {
		if (!(dup2(fd, target) == target))
			pc_unresolved("dup2 to %d failed: %s", target, pc_errname(errno));
		pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
	}
}

int main(void)
{
	pc_begin("XSH.getchar.005");
	{
		int r, e;

		pc_make_file("f", "0123");
		fd_onto("f", O_WRONLY, 0);
		pc_test();
		errno = 0;
		r = getchar();
		e = errno;
		pc_check(r == EOF, "getchar() on a write-only descriptor returned %d, expected an error", r);
		pc_check(e == EBADF, "getchar() set errno %s, expected EBADF", pc_errname(e));
		pc_check(ferror(stdin) != 0, "the error indicator of stdin is not set");
	}
	pc_pass();
}
