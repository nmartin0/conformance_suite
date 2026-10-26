/*
 * fputc() writes at the stream's position.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputc.003
 * MUTANT    fputc.at_end
 * VARIANT   setvbuf.tiny_default
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

int main(void)
{
	pc_begin("XSH.fputc.003");
	{
		FILE *f;
		char buf[32];
		int r;

		pc_make_file("f", "0123456789");
		pc_test();
		f = open_as("f", "r+");
		NEED(fseek(f, 3, SEEK_SET) == 0, "fseek() failed: %s", pc_errname(errno));
		r = fputc('x', f);
		pc_check(r == 'x', "fputc('x') returned %d", r);
		r = fputc('y', f);
		pc_check(r == 'y', "fputc('y') returned %d", r);
		NEED(fclose(f) == 0, "fclose() failed: %s", pc_errname(errno));
		(void)slurp("f", buf, sizeof buf);
		pc_check(strcmp(buf, "012xy56789") == 0, "the file holds \"%s\", expected \"012xy56789\"", buf);
	}
	pc_pass();
}
