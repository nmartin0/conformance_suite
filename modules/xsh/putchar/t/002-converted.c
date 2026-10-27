/*
 * putchar() converts its argument to unsigned char.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.putchar.002
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

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
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
	pc_begin("XSH.putchar.002");
	{
		static const unsigned char want[3] = { 0x41, 0xff, 0x00 };
		char buf[16];
		long n;
		int r;

		fd_onto("out", O_RDWR | O_CREAT | O_TRUNC, 1);
		pc_test();
		r = putchar(0x141);
		pc_check(r == 0x41, "putchar(0x141) returned %d, expected 65", r);
		r = putchar(-1);
		pc_check(r == 0xff, "putchar(-1) returned %d, expected 255", r);
		r = putchar(0x100);
		pc_check(r == 0, "putchar(0x100) returned %d, expected 0", r);
		NEED(fflush(stdout) == 0, "fflush(stdout) failed: %s", pc_errname(errno));
		n = slurp("out", buf, sizeof buf);
		pc_check(n == 3 && memcmp(buf, want, 3) == 0, "the file holds %ld bytes: %s, expected 41 ff 00", n, hex(buf, (size_t)n));
	}
	pc_pass();
}
