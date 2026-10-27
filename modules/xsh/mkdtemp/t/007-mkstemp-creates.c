/*
 * mkstemp() makes a regular file and returns a descriptor for it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mkdtemp.007
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)


int main(void)
{
	char t[] = "fXXXXXX";
	struct stat a, b;
	int fd;

	pc_begin("XSH.mkdtemp.007");
	pc_require_space();
	pc_test();
	fd = mkstemp(t);
	pc_expect_ok(fd, "mkstemp()");
	pc_check(stat(t, &a) == 0, "nothing exists at the name %s", t);
	pc_check(fstat(fd, &b) == 0, "fstat failed");
	pc_check(S_ISREG(a.st_mode), "%s is not a regular file", t);
	pc_check(a.st_size == 0, "%s has length %ld", t, (long)a.st_size);
	pc_check(a.st_dev == b.st_dev && a.st_ino == b.st_ino, "the descriptor is not for the file named %s", t);
	pc_pass();
}
