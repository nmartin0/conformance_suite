/*
 * fputws() followed by fflush() marks the modification time for update.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputws.007
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

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
	pc_begin("XSH.fputws.007");
	{
		FILE *f;
		int r;

		pc_make_file("f", "0123");
		f = fopen("f", "r+");
		pc_check(f != NULL, "fopen() failed: %s", pc_errname(errno));
		old_mtime(fileno(f));
		pc_test();
		r = fputws(L"x", f);
		pc_check(r >= 0, "fputws() returned %d", r);
		NEED(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		pc_check(mtime_moved(fileno(f)), "st_mtim is still 2001-09-09 after fputws() and fflush()");
	}
	pc_pass();
}
