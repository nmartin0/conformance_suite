/*
 * perror() marks the modification time of the file behind stderr for update.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.perror.008
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
	pc_begin("XSH.perror.008");
	{
		struct timespec ts[2];
		struct stat st;
		int keep;

		keep = dup(2);
		pc_check(keep >= 0, "dup(2) failed: %s", pc_errname(errno));
		fd_onto("err", O_RDWR | O_CREAT | O_TRUNC, 2);
		ts[0].tv_sec = 0;
		ts[0].tv_nsec = UTIME_OMIT;
		ts[1].tv_sec = 1000000000;
		ts[1].tv_nsec = 0;
		pc_check(futimens(2, ts) == 0, "futimens failed: %s", pc_errname(errno));
		pc_test();
		errno = ENOENT;
		perror("p");
		NEED(fflush(stderr) == 0, "fflush(stderr) failed: %s", pc_errname(errno));
		NEED(fstat(2, &st) == 0, "fstat failed: %s", pc_errname(errno));
		NEED(dup2(keep, 2) == 2, "restoring descriptor 2 failed: %s", pc_errname(errno));
		pc_check(st.st_mtim.tv_sec > 1000000000, "st_mtim is still 2001-09-09 after perror() and fflush(stderr)");
	}
	pc_pass();
}
