/*
 * freopen() opens the descriptor with fopen()'s flags for the mode, and no others.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.freopen.007
 * MUTANT    freopen.rdwr_always
 * MUTANT    freopen.cloexec_always
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

static const struct { const char *mode; int acc, append; } want[] = {
	{ "r", O_RDONLY, 0 }, { "w", O_WRONLY, 0 }, { "a", O_WRONLY, 1 },
	{ "r+", O_RDWR, 0 }, { "w+", O_RDWR, 0 }, { "a+", O_RDWR, 1 },
};

int main(void)
{
	pc_begin("XSH.freopen.007");
	{
		FILE *f;
		int i, fl, fd;

		pc_make_file("x", "data");
		f = open_as("x", "r");
		pc_test();
		for (i = 0; i < 6; i++) {
			pc_check(freopen("x", want[i].mode, f) == f, "freopen(\"x\", \"%s\") failed: %s", want[i].mode,
			    pc_errname(errno));
			fl = fcntl(fileno(f), F_GETFL);
			fd = fcntl(fileno(f), F_GETFD);
			NEED(fl != -1 && fd != -1, "fcntl failed: %s", pc_errname(errno));
			pc_check((fl & O_ACCMODE) == want[i].acc, "\"%s\": access mode %#x, expected %#x", want[i].mode,
			    fl & O_ACCMODE, want[i].acc);
			pc_check(((fl & O_APPEND) != 0) == want[i].append, "\"%s\": O_APPEND is %s", want[i].mode,
			    (fl & O_APPEND) ? "set" : "clear");
			pc_check((fl & O_NONBLOCK) == 0, "\"%s\": O_NONBLOCK is set", want[i].mode);
#ifdef O_SYNC
			pc_check((fl & O_SYNC) != O_SYNC, "\"%s\": O_SYNC is set", want[i].mode);
#endif
#ifdef O_DSYNC
			pc_check((fl & O_DSYNC) == 0, "\"%s\": O_DSYNC is set", want[i].mode);
#endif
			pc_check((fd & FD_CLOEXEC) == 0, "\"%s\": FD_CLOEXEC is set", want[i].mode);
		}
	}
	pc_pass();
}
