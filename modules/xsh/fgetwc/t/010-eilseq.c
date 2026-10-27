/*
 * fgetwc() fails with EILSEQ on bytes that are not a valid character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetwc.010
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

/* Setup: switches to an installed UTF-8 locale, or the test is not in use. */
static void utf8(void)
{
	const char *name = pc_find_locale("UTF-8", 1);

	if (name == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_ALL, name) == NULL)
		pc_unresolved("setlocale(%s) failed", name);
}


int main(void)
{
	pc_begin("XSH.fgetwc.010");
	{
		FILE *f;
		wint_t r;
		int e;

		utf8();
		make_bytes("f", "\xff", 1);
		pc_test();
		f = open_as("f", "r");
		errno = 0;
		r = fgetwc(f);
		e = errno;
		pc_check(r == WEOF, "fgetwc() on the invalid byte 0xff returned %#lx, expected WEOF", (unsigned long)r);
		pc_check(e == EILSEQ, "fgetwc() set errno %s, expected EILSEQ", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set after an encoding error");
	}
	pc_pass();
}
