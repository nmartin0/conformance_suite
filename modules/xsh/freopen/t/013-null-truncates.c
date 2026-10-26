/*
 * freopen(NULL, "w", f) acts as if the file's name were used: it truncates.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.freopen.013
 * MUTANT    freopen.null_no_reopen
 * VARIANT   freopen.null_refused
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
	pc_begin("XSH.freopen.013");
	{
		char buf[16];
		FILE *f, *g;
		long p;

		f = open_as("f", "w");
		pc_check(fputs("abc", f) >= 0 && fflush(f) == 0 && pc_file_size("f") == 3, "writing failed");
		pc_test();
		g = freopen(NULL, "w", f);
		if (g == NULL)
			pc_notinuse("freopen(NULL, \"w\") on a \"w\" stream is not permitted here (%s)", pc_errname(errno));
		pc_check(g == f, "freopen(NULL, \"w\") returned a pointer other than the stream it was given");
		pc_check(pc_file_size("f") == 0, "freopen(NULL, \"w\") left the file %ld bytes long", pc_file_size("f"));
		p = ftell(f);
		pc_check(p == 0, "ftell() after freopen(NULL, \"w\") is %ld", p);
		NEED(fputs("Z", f) >= 0 && fflush(f) == 0, "writing failed: %s", pc_errname(errno));
		(void)slurp("f", buf, sizeof buf);
		pc_check(strcmp(buf, "Z") == 0, "after writing \"Z\" the file holds \"%s\"", buf);
	}
	pc_pass();
}
