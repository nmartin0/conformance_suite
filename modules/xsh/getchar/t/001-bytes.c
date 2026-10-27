/*
 * getchar() reads the bytes of standard input in order and then returns EOF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getchar.001
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
	pc_begin("XSH.getchar.001");
	{
		int i, c;
		long pos;

		make_bytes("f", four, sizeof four);
		fd_onto("f", O_RDONLY, 0);
		pc_test();
		for (i = 0; i < 4; i++) {
			c = getchar();
			pc_check(c == four[i], "getchar() number %d returned %d, expected %d", i + 1, c, four[i]);
			pos = ftell(stdin);
			NEED(pos != -1, "ftell() failed: %s", pc_errname(errno));
			pc_check(pos == i + 1, "ftell(stdin) after %d reads is %ld", i + 1, pos);
		}
		c = getchar();
		pc_check(c == EOF, "getchar() at the end of the file returned %d, expected EOF", c);
		pc_check(feof(stdin) != 0, "the end-of-file indicator of stdin is not set");
	}
	pc_pass();
}
