/*
 * puts() writes the string and a newline to stdout and returns a non-negative value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.puts.001
 * MUTANT    puts.no_newline
 * MUTANT    puts.writes_nul
 * MUTANT    puts.returns_eof
 * VARIANT   setvbuf.tiny_default
 * VARIANT   puts.returns_count
 * VARIANT   puts.returns_last
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

/* Puts a regular file path on descriptor 1, for stdout (setup). */
static void stdout_to(const char *path)
{
	int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	pc_check(dup2(fd, 1) == 1, "dup2 failed: %s", pc_errname(errno));
	pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
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
	pc_begin("XSH.puts.001");
	{
		char hello[] = "hello", empty[] = "";
		char buf[16];
		long n;
		int r;

		stdout_to("out");
		pc_test();
		r = puts(hello);
		pc_check(r >= 0, "puts(\"hello\") returned %d, expected a non-negative value", r);
		r = puts(empty);
		pc_check(r >= 0, "puts(\"\") returned %d, expected a non-negative value", r);
		NEED(fflush(stdout) == 0, "fflush(stdout) failed: %s", pc_errname(errno));
		n = slurp("out", buf, sizeof buf);
		pc_check(n == 7 && memcmp(buf, "hello\n\n", 7) == 0, "the file holds %ld bytes: %s, expected 68 65 6c 6c 6f 0a 0a", n, hex(buf, (size_t)n));
	}
	pc_pass();
}
