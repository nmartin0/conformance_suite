/*
 * clearerr() clears the end-of-file and the error indicator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.clearerr.001
 * MUTANT    clearerr.eof_only
 * MUTANT    clearerr.error_only
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
	pc_begin("XSH.clearerr.001");
	{
		FILE *f, *g;
		int p[2];

		pc_make_file("f", "a");
		f = open_as("f", "r");
		pc_check(fgetc(f) == 'a' && fgetc(f) == EOF && feof(f), "cannot reach end-of-file");
		/* g: both indicators, from an EAGAIN and then end-of-file. */
		pc_check(pipe(p) == 0 && fcntl(p[0], F_SETFL, O_NONBLOCK) == 0, "pipe or fcntl failed");
		g = fdopen(p[0], "r");
		pc_check(g != NULL, "fdopen failed");
		pc_check(fgetc(g) == EOF && ferror(g), "fgetc() on an empty O_NONBLOCK pipe did not set the error indicator");
		pc_check(close(p[1]) == 0, "close failed");
		pc_check(fgetc(g) == EOF, "fgetc() after the writer closed returned a byte");
		if (!feof(g) || !ferror(g))
			pc_unresolved("the pipe stream does not have both indicators set (feof %d, ferror %d)",
			    feof(g) != 0, ferror(g) != 0);
		pc_test();
		clearerr(f);
		pc_check(feof(f) == 0, "feof() is non-zero after clearerr()");
		pc_check(ferror(f) == 0, "ferror() is non-zero after clearerr()");
		clearerr(g);
		pc_check(feof(g) == 0, "with both indicators set, feof() is non-zero after clearerr()");
		pc_check(ferror(g) == 0, "with both indicators set, ferror() is non-zero after clearerr()");
	}
	pc_pass();
}
