/*
 * fgetwc() at end of file returns WEOF and sets the end-of-file indicator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetwc.003
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
	pc_begin("XSH.fgetwc.003");
	{
		FILE *f, *g;
		wint_t c;

		make_bytes("e", "", 0);
		pc_make_file("f", "ab");
		pc_test();
		f = open_as("e", "r");
		c = fgetwc(f);
		pc_check(c == WEOF, "fgetwc() on an empty file returned %#lx, expected WEOF", (unsigned long)c);
		pc_check(feof(f) != 0, "feof() is false after fgetwc() reached the end of an empty file");
		pc_check(ferror(f) == 0, "ferror() is true after the end of an empty file");
		g = open_as("f", "r");
		NEED(fgetwc(g) == L'a' && fgetwc(g) == L'b', "the first two reads of \"ab\" did not return L'a' and L'b'");
		c = fgetwc(g);
		pc_check(c == WEOF, "fgetwc() after the last character returned %#lx, expected WEOF", (unsigned long)c);
		pc_check(feof(g) != 0, "feof() is false after fgetwc() reached the end of the file");
		pc_check(ferror(g) == 0, "ferror() is true after the end of the file");
	}
	pc_pass();
}
