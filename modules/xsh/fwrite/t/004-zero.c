/*
 * fwrite() with size or nitems 0 returns 0 and leaves the stream unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwrite.004
 * MUTANT    fwrite.zero_writes
 * MUTANT    fwrite.zero_clears
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
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

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

static int ts_after(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec);
}

/*
 * Set the error indicator of a stream that has nothing buffered for input:
 * its descriptor is closed for one fgetc(), whose read fails with EBADF
 * (XSH fgetc(): data needs to be read), then put back with dup2(). The
 * copy is never used to affect the offset, so it is not a second handle
 * (XSH 2.5.1).
 */
static void set_error(FILE *f)
{
	int fd = fileno(f), d = dup(fd);

	NEED(fd >= 0 && d >= 0, "dup failed: %s", pc_errname(errno));
	NEED(close(fd) == 0, "close failed");
	NEED(fgetc(f) == EOF, "fgetc() on a closed descriptor returned a byte");
	NEED(dup2(d, fd) == fd && close(d) == 0, "dup2 failed: %s", pc_errname(errno));
	NEED(ferror(f), "fgetc() on a closed descriptor did not set the error indicator");
}

int main(void)
{
	pc_begin("XSH.fwrite.004");
	{
		char buf[16];
		size_t r;
		long p;
		FILE *f, *x;
		int fd, d;

		/* x: a "w" stream whose error indicator is set by a flush to its closed descriptor. */
		x = open_as("x", "w");
		fd = fileno(x);
		d = dup(fd);
		pc_check(d >= 0 && close(fd) == 0, "dup or close failed");
		(void)fputs("ab", x);	/* with a small buffer this may already fail */
		(void)fflush(x);
		pc_check(dup2(d, fd) == fd && close(d) == 0, "dup2 failed");
		pc_check(ferror(x) != 0, "writing to a closed descriptor did not set the error indicator");
		pc_test();
		f = open_as("f", "w");
		NEED(fputs("ab", f) >= 0, "fputs failed");
		r = fwrite("xyz", 0, 3, f);
		pc_check(r == 0, "fwrite(b, 0, 3) returned %zu", r);
		r = fwrite("xyz", 3, 0, f);
		pc_check(r == 0, "fwrite(b, 3, 0) returned %zu", r);
		p = ftell(f);
		pc_check(p == 2, "ftell() is %ld after the zero-sized writes, expected 2", p);
		pc_check(ferror(f) == 0, "the error indicator was set");
		NEED(fclose(f) == 0, "fclose failed");
		(void)slurp("f", buf, sizeof buf);
		pc_check(strcmp(buf, "ab") == 0, "the file holds \"%s\", expected \"ab\"", buf);
		pc_check(fwrite("xyz", 0, 3, x) == 0 && fwrite("xyz", 3, 0, x) == 0, "a zero-sized fwrite() returned non-zero");
		pc_check(ferror(x) != 0, "a zero-sized fwrite() cleared the error indicator");
	}
	pc_pass();
}
