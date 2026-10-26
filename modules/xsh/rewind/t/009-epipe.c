/*
 * rewind() flushing to a pipe with no reader sets errno to EPIPE (or ESPIPE).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.rewind.009
 * MUTANT    rewind.wrong_errno
 * MUTANT    rewind.errno_hidden
 * MUTANT    rewind.clears_first
 * VARIANT   rewind.lseek_first
 * VARIANT   setvbuf.own_unused
 * VARIANT   setvbuf.scribbles
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
	pc_begin("XSH.rewind.009");
	{
		static char vb[4096];
		sigset_t s;
		FILE *f;
		int p[2], e;

		sigemptyset(&s);
		sigaddset(&s, SIGPIPE);
		if (!(sigprocmask(SIG_BLOCK, &s, NULL) == 0))
			pc_unresolved("sigprocmask failed");
		pc_check(pipe(p) == 0 && close(p[0]) == 0, "pipe failed");
		f = fdopen(p[1], "w");
		pc_check(f != NULL && setvbuf(f, vb, _IOFBF, sizeof vb) == 0, "fdopen or setvbuf failed");
		pc_test();	/* its fflush(NULL) would write the data meant to stay buffered */
		/* Written at once, this would have failed with EPIPE. */
		NEED(fputs("abc", f) >= 0, "fputs failed: the stream wrote at once");
		errno = 0;
		rewind(f);
		e = errno;
		pc_check(e == EPIPE || e == ESPIPE, "rewind() left errno %s, expected EPIPE or ESPIPE", pc_errname(e));
		if (e == EPIPE) {
			NEED(sigpending(&s) == 0, "sigpending failed");
			pc_check(sigismember(&s, SIGPIPE) == 1, "EPIPE, but SIGPIPE is not pending");
		}
		pc_check(ferror(f) == 0, "the error indicator is set after rewind()");
	}
	pc_pass();
}
