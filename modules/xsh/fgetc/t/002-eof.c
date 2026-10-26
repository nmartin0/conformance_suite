/*
 * fgetc() at end of file returns EOF and sets the end-of-file indicator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetc.002
 * MUTANT    fgetc.eof_no_indicator
 * MUTANT    fgetc.eof_as_byte
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

/* A file holding exactly the len bytes at data (setup). */
static void make_bytes(const char *path, const void *data, size_t len)
{
	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	pc_check(len == 0 || write(fd, data, len) == (ssize_t)len, "write(%s) failed: %s", path, pc_errname(errno));
	pc_check(close(fd) == 0, "close(%s) failed: %s", path, pc_errname(errno));
}

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

int main(void)
{
	pc_begin("XSH.fgetc.002");
	{
		FILE *f, *g;
		int c;

		make_bytes("e", "", 0);
		pc_make_file("f", "ab");
		pc_test();
		f = open_as("e", "r");
		c = fgetc(f);
		pc_check(c == EOF, "fgetc() on an empty file returned %d, expected EOF", c);
		pc_check(feof(f) != 0, "feof() is false after fgetc() reached the end of an empty file");
		pc_check(ferror(f) == 0, "ferror() is true after the end of an empty file");
		g = open_as("f", "r");
		NEED(fgetc(g) == 'a' && fgetc(g) == 'b', "the first two reads of \"ab\" did not return 'a' and 'b'");
		c = fgetc(g);
		pc_check(c == EOF, "fgetc() after the last byte returned %d, expected EOF", c);
		pc_check(feof(g) != 0, "feof() is false after fgetc() reached the end of the file");
		pc_check(ferror(g) == 0, "ferror() is true after the end of the file");
	}
	pc_pass();
}
