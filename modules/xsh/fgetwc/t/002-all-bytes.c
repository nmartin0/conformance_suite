/*
 * Every byte value is a valid character of the POSIX locale for fgetwc().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetwc.002
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

/* A file holding exactly the len bytes at data (setup). */
static void make_bytes(const char *path, const void *data, size_t len)
{
	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	pc_check(len == 0 || write(fd, data, len) == (ssize_t)len, "write(%s) failed: %s", path, pc_errname(errno));
	pc_check(close(fd) == 0, "close(%s) failed: %s", path, pc_errname(errno));
}

int main(void)
{
	pc_begin("XSH.fgetwc.002");
	{
		unsigned char all[255];
		FILE *f;
		wint_t c;
		int i;

		for (i = 0; i < 255; i++)
			all[i] = (unsigned char)(i + 1);
		make_bytes("f", all, sizeof all);
		pc_test();
		f = open_as("f", "r");
		for (i = 0; i < 255; i++) {
			errno = 0;
			c = fgetwc(f);
			pc_check(c != WEOF, "fgetwc() returned WEOF for byte %#x (errno %s)", i + 1, pc_errname(errno));
		}
		pc_check(ferror(f) == 0, "the error indicator is set after the 255 reads");
		c = fgetwc(f);
		pc_check(c == WEOF, "fgetwc() after the last byte returned %#lx, expected WEOF", (unsigned long)c);
	}
	pc_pass();
}
