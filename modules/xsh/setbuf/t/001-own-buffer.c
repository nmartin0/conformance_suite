/*
 * setbuf() with a buffer: the data arrives intact and the array is not used past BUFSIZ bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setbuf.001
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
	pc_begin("XSH.setbuf.001");
	{
		static char arr[BUFSIZ + 64];
		static char data[2 * BUFSIZ + 100];
		static char got[2 * BUFSIZ + 110];
		FILE *f;
		long n;
		size_t i;

		memset(arr, '#', sizeof arr);
		for (i = 0; i < sizeof data; i++)
			data[i] = (char)('a' + i % 26);
		pc_test();
		f = open_as("f", "w");
		setbuf(f, arr);
		for (i = 0; i < sizeof data; i++)
			NEED(fputc(data[i], f) == (unsigned char)data[i], "fputc() number %lu failed", (unsigned long)i + 1);
		NEED(fclose(f) == 0, "fclose() failed: %s", pc_errname(errno));
		n = slurp("f", got, sizeof got);
		pc_check(n == (long)sizeof data && memcmp(got, data, sizeof data) == 0, "the file holds %ld bytes, expected %lu, or its contents differ", n, (unsigned long)sizeof data);
		for (i = BUFSIZ; i < sizeof arr; i++)
			pc_check(arr[i] == '#', "byte %lu of the array, past BUFSIZ, was changed", (unsigned long)i);
	}
	pc_pass();
}
