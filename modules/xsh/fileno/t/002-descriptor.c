/*
 * fileno() returns the stream's own descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fileno.002
 * MUTANT    fileno.off_by_one
 * MUTANT    fileno.lowest
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
	pc_begin("XSH.fileno.002");
	{
		struct stat a, b;
		FILE *f, *g;
		int fd, d, k, fl;

		pc_make_file("f", "abc");
		pc_make_file("g", "");
		f = open_as("f", "r");
		fd = open("g", O_WRONLY);
		pc_check(fd >= 0, "open failed");
		d = fcntl(fd, F_DUPFD, 40);
		pc_check(d >= 40 && close(fd) == 0, "fcntl(F_DUPFD) failed");
		g = fdopen(d, "w");
		pc_check(g != NULL, "fdopen failed: %s", pc_errname(errno));
		pc_check(stat("f", &a) == 0, "stat failed");
		pc_test();
		k = fileno(f);
		pc_check(k >= 0, "fileno() of a stream from fopen() is %d", k);
		pc_check(fstat(k, &b) == 0 && a.st_dev == b.st_dev && a.st_ino == b.st_ino,
		    "fileno() returned %d, which is not a descriptor for the stream's file", k);
		fl = fcntl(k, F_GETFL);
		pc_check(fl != -1 && (fl & O_ACCMODE) == O_RDONLY, "descriptor %d is not open O_RDONLY (flags %#x)", k, fl);
		k = fileno(g);
		pc_check(k == d, "fileno() of fdopen(%d) is %d", d, k);
	}
	pc_pass();
}
