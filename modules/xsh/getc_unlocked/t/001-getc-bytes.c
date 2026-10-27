/*
 * getc_unlocked() reads the bytes of a stream in order under flockfile().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getc_unlocked.001
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

/* Creates path holding the n bytes of data (setup). */
static void make_bytes(const char *path, const void *data, size_t n)
{
	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	pc_check(write(fd, data, n) == (ssize_t)n, "write(%s) failed: %s", path, pc_errname(errno));
	pc_check(close(fd) == 0, "close(%s) failed: %s", path, pc_errname(errno));
}

static const unsigned char four[4] = { 0x41, 0xff, 0x00, 0x42 };

int main(void)
{
	pc_begin("XSH.getc_unlocked.001");
	{
		FILE *f;
		int i, c;
		long pos;

		make_bytes("f", four, sizeof four);
		pc_test();
		f = open_as("f", "r");
		flockfile(f);
		for (i = 0; i < 4; i++) {
			c = getc_unlocked(f);
			pc_check(c == four[i], "getc_unlocked() number %d returned %d, expected %d", i + 1, c, four[i]);
			pos = ftell(f);
			NEED(pos != -1, "ftell() failed: %s", pc_errname(errno));
			pc_check(pos == i + 1, "ftell() after %d reads is %ld", i + 1, pos);
		}
		c = getc_unlocked(f);
		pc_check(c == EOF, "getc_unlocked() at the end of the file returned %d, expected EOF", c);
		pc_check(feof(f) != 0, "the end-of-file indicator is not set");
		funlockfile(f);
		(void)fclose(f);
	}
	pc_pass();
}
