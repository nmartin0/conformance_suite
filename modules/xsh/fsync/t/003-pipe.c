/*
 * fsync() of a pipe returns 0 or fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fsync.003
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
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	int p[2], r, e;

	pc_begin("XSH.fsync.003");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	r = fsync(p[0]);
	e = errno;
	if (r == 0)
		pc_pass();
	pc_check(r == -1, "fsync() returned %d", r);
	pc_check(e == EINVAL, "fsync() of a pipe failed with %s, not EINVAL", pc_errname(e));
	pc_pass();
}
