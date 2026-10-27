/*
 * fputws() of an empty string writes nothing and succeeds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputws.002
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

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
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
	pc_begin("XSH.fputws.002");
	{
		char buf[8];
		FILE *f;
		int r;
		long n;

		pc_test();
		f = open_as("f", "w");
		r = fputws(L"", f);
		pc_check(r >= 0, "fputws(L\"\") returned %d, expected a non-negative value", r);
		pc_check(fputws(L"a", f) >= 0 && fputws(L"b", f) >= 0, "a later fputws() failed");
		pc_check(fclose(f) == 0, "fclose() failed: %s", pc_errname(errno));
		n = slurp("f", buf, sizeof buf);
		pc_check(n == 2 && memcmp(buf, "ab", 2) == 0, "the file holds %ld bytes %s, expected \"ab\"", n, hex(buf, (size_t)n));
	}
	pc_pass();
}
