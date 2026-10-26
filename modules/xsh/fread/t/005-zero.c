/*
 * fread() with size or nitems 0 returns 0 and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fread.005
 * MUTANT    fread.zero_reads
 * MUTANT    fread.zero_clears
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

static int untouched(const char *b, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		if (b[i] != 'Z')
			return 0;
	return 1;
}

int main(void)
{
	pc_begin("XSH.fread.005");
	{
		char b[8];
		size_t r;
		long p;
		FILE *f, *e, *x;
		int c;

		pc_make_file("f", "0123456789");
		pc_make_file("e", "a");
		e = open_as("e", "r");
		NEED(fgetc(e) == 'a' && fgetc(e) == EOF && feof(e), "cannot reach end-of-file");
		x = open_as("f", "r");
		set_error(x);
		pc_test();
		f = open_as("f", "r");
		NEED(fgetc(f) == '0' && fgetc(f) == '1', "reading failed");
		memset(b, 'Z', sizeof b);
		r = fread(b, 0, 5, f);
		pc_check(r == 0, "fread(b, 0, 5) returned %zu", r);
		r = fread(b, 5, 0, f);
		pc_check(r == 0, "fread(b, 5, 0) returned %zu", r);
		pc_check(untouched(b, sizeof b), "the array was changed");
		p = ftell(f);
		pc_check(p == 2, "ftell() is %ld after the zero-sized reads, expected 2", p);
		pc_check(!feof(f) && !ferror(f), "an indicator was set (feof %d, ferror %d)", feof(f) != 0, ferror(f) != 0);
		c = fgetc(f);
		pc_check(c == '2', "the next fgetc() returned %d, expected '2'", c);
		pc_check(fread(b, 1, 0, e) == 0 && fread(b, 0, 1, e) == 0, "a zero-sized fread() at end-of-file returned non-zero");
		pc_check(feof(e) != 0, "a zero-sized fread() cleared the end-of-file indicator");
		pc_check(fread(b, 1, 0, x) == 0 && fread(b, 0, 1, x) == 0, "a zero-sized fread() returned non-zero");
		pc_check(ferror(x) != 0, "a zero-sized fread() cleared the error indicator");
		pc_check(untouched(b, sizeof b), "the array was changed");
	}
	pc_pass();
}
