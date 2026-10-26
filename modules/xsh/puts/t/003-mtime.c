/*
 * puts() marks the modification time for update by the next fflush().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.puts.003
 * MUTANT    puts.no_mtime
 * VARIANT   setvbuf.tiny_default
 * VARIANT   puts.returns_count
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

/* Puts a regular file path on descriptor 1, for stdout (setup). */
static void stdout_to(const char *path)
{
	int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	if (!(dup2(fd, 1) == 1))
		pc_unresolved("dup2 failed: %s", pc_errname(errno));
	pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
}

/* Sets the descriptor's file's modification time to 2001-09-09 (setup). */
static void old_mtime(int fd)
{
	struct timespec ts[2];

	ts[0].tv_sec = 0;
	ts[0].tv_nsec = UTIME_OMIT;
	ts[1].tv_sec = 1000000000;
	ts[1].tv_nsec = 0;
	pc_check(futimens(fd, ts) == 0, "futimens failed: %s", pc_errname(errno));
}

/* The descriptor's file has a modification time later than 2001-09-09. */
static int mtime_moved(int fd)
{
	struct stat st;

	NEED(fstat(fd, &st) == 0, "fstat failed: %s", pc_errname(errno));
	return st.st_mtim.tv_sec > 1000000000;
}

int main(void)
{
	pc_begin("XSH.puts.003");
	{
		char x[] = "x";
		int r;

		stdout_to("out");
		old_mtime(1);
		pc_test();
		r = puts(x);
		pc_check(r >= 0, "puts() returned %d", r);
		NEED(fflush(stdout) == 0, "fflush(stdout) failed: %s", pc_errname(errno));
		pc_check(mtime_moved(1), "st_mtim is still 2001-09-09 after puts() and fflush(stdout)");
	}
	pc_pass();
}
