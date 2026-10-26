/*
 * fsetpos() returns the stream to a position stored by fgetpos().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fsetpos.001
 * MUTANT    fsetpos.noop
 * MUTANT    fsetpos.off_by_one
 * VARIANT   setvbuf.tiny_default
 * VARIANT   fgetpos.opaque
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
	pc_begin("XSH.fsetpos.001");
	{
		fpos_t p0, p3;
		FILE *f;
		int r, c;

		pc_make_file("f", "0123456789");
		pc_test();
		f = open_as("f", "r");
		NEED(fgetpos(f, &p0) == 0, "fgetpos failed");
		NEED(fgetc(f) == '0' && fgetc(f) == '1' && fgetc(f) == '2', "reading failed");
		NEED(fgetpos(f, &p3) == 0, "fgetpos failed");
		NEED(fgetc(f) == '3' && fgetc(f) == '4', "reading failed");
		r = fsetpos(f, &p3);
		pc_check(r == 0, "fsetpos() returned %d: %s", r, pc_errname(errno));
		c = fgetc(f);
		pc_check(c == '3', "after fsetpos() to the value stored at offset 3 the read returned %d", c);
		r = fsetpos(f, &p0);
		pc_check(r == 0, "fsetpos() returned %d: %s", r, pc_errname(errno));
		c = fgetc(f);
		pc_check(c == '0', "after fsetpos() to the value stored at the start the read returned %d", c);
	}
	pc_pass();
}
