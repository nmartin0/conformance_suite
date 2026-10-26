/*
 * The first fgets() that returns data marks the access time for update.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgets.007
 * MUTANT    fgets.no_atime
 * VARIANT   setvbuf.tiny_default
 * VARIANT   fopen.read_all
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

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

/* Sets path's last data access time to 2001-09-09, leaving the modification time (setup). */
static void old_atime(const char *path)
{
	struct timespec ts[2];

	ts[0].tv_sec = 1000000000;
	ts[0].tv_nsec = 0;
	ts[1].tv_sec = 0;
	ts[1].tv_nsec = UTIME_OMIT;
	pc_check(utimensat(AT_FDCWD, path, ts, 0) == 0, "utimensat(%s) failed: %s", path, pc_errname(errno));
}

/* The descriptor's file has an access time later than 2001-09-09. */
static int atime_moved(int fd)
{
	struct stat st;

	NEED(fstat(fd, &st) == 0, "fstat failed: %s", pc_errname(errno));
	return st.st_atim.tv_sec > 1000000000;
}

int main(void)
{
	pc_begin("XSH.fgets.007");
	{
		FILE *f;
		char s[16];

		pc_make_file("f", "01\n23\n");
		old_atime("f");
		pc_test();
		f = open_as("f", "r");
		pc_check(fgets(s, 16, f) == s && strcmp(s, "01\n") == 0, "fgets() did not return the first line");
		pc_check(atime_moved(fileno(f)), "st_atim is still 2001-09-09 after fgets() (is the file system mounted noatime?)");
	}
	pc_pass();
}
