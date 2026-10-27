/*
 * fwprintf() and wprintf() make their stream wide-oriented.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwprintf.011
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

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

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
	pc_begin("XSH.fwprintf.011");
	{
		FILE *f;
		int r;

		fd_onto("o", O_RDWR | O_CREAT | O_TRUNC, 1);
		pc_test();
		f = open_as("f", "w");
		r = fwprintf(f, L"x");
		NEED(r >= 0, "fwprintf() failed: %s", pc_errname(errno));
		pc_check(fwide(f, 0) > 0, "the stream is not wide-oriented after fwprintf()");
		r = wprintf(L"x");
		NEED(r >= 0, "wprintf() failed: %s", pc_errname(errno));
		pc_check(fwide(stdout, 0) > 0, "stdout is not wide-oriented after wprintf()");
	}
	pc_pass();
}
