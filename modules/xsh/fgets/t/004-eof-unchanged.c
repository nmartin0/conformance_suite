/*
 * fgets() at end of file returns a null pointer and leaves the array alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgets.004
 * MUTANT    fgets.eof_clobbers
 * MUTANT    fgets.eof_returns_s
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

/* Up to 16 bytes as hex, for messages. */
static const char *hex(const void *p, size_t n)
{
	static char out[16 * 3 + 1];
	const unsigned char *b = p;
	size_t i;

	out[0] = '\0';
	for (i = 0; i < n && i < 16; i++)
		snprintf(out + 3 * i, sizeof out - 3 * i, "%02x ", b[i]);
	return out;
}

int main(void)
{
	pc_begin("XSH.fgets.004");
	{
		FILE *f, *g;
		char s[16], q[16], *p;

		make_bytes("e", "", 0);
		pc_make_file("f", "ab\n");
		memset(q, 'Q', sizeof q);
		pc_test();
		f = open_as("e", "r");
		memcpy(s, q, sizeof s);
		p = fgets(s, 16, f);
		pc_check(p == NULL, "fgets() on an empty file returned %p, expected a null pointer", (void *)p);
		pc_check(feof(f) != 0, "feof() is false after fgets() met the end of an empty file");
		pc_check(memcmp(s, q, sizeof s) == 0, "fgets() on an empty file changed the array: %s", hex(s, 16));
		g = open_as("f", "r");
		NEED(fgets(s, 16, g) == s, "the first fgets() of \"ab\\n\" failed");
		memcpy(s, q, sizeof s);
		p = fgets(s, 16, g);
		pc_check(p == NULL, "fgets() after the last line returned %p, expected a null pointer", (void *)p);
		pc_check(feof(g) != 0, "feof() is false after fgets() met the end of the file");
		pc_check(memcmp(s, q, sizeof s) == 0, "fgets() at end of file changed the array: %s", hex(s, 16));
	}
	pc_pass();
}
