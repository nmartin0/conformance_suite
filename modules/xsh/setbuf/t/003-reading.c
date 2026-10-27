/*
 * setbuf() on a stream that is read: the bytes arrive intact and in order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setbuf.003
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
	pc_begin("XSH.setbuf.003");
	{
		static char arr[BUFSIZ];
		FILE *f;
		int pass, i, c;

		pc_make_file("f", "0123");
		pc_test();
		for (pass = 0; pass < 2; pass++) {
			f = open_as("f", "r");
			setbuf(f, pass == 0 ? arr : NULL);
			for (i = 0; i < 4; i++) {
				c = getc(f);
				pc_check(c == '0' + i, "%s: read number %d returned %d, expected %d", pass == 0 ? "own buffer" : "null buffer", i + 1, c, '0' + i);
			}
			c = getc(f);
			pc_check(c == EOF, "%s: the read at the end returned %d, expected EOF", pass == 0 ? "own buffer" : "null buffer", c);
			NEED(fclose(f) == 0, "fclose() failed: %s", pc_errname(errno));
		}
	}
	pc_pass();
}
