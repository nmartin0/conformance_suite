/*
 * fwrite() to a pipe with no reader fails with EPIPE and SIGPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwrite.006
 * MUTANT    fwrite.no_error_flag
 * MUTANT    fwrite.wrong_errno
 * MUTANT    fwrite.error_full_count
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

#define MIB 1048576

int main(void)
{
	pc_begin("XSH.fwrite.006");
	{
		static char b[MIB];
		sigset_t s;
		size_t r;
		FILE *f;
		int p[2], e;

		memset(b, 'x', sizeof b);
		sigemptyset(&s);
		sigaddset(&s, SIGPIPE);
		if (!(sigprocmask(SIG_BLOCK, &s, NULL) == 0))
			pc_unresolved("sigprocmask failed");
		pc_check(pipe(p) == 0 && close(p[0]) == 0, "pipe failed");
		pc_test();
		f = fdopen(p[1], "w");
		NEED(f != NULL, "fdopen failed: %s", pc_errname(errno));
		errno = 0;
		r = fwrite(b, 1, MIB, f);
		e = errno;
		pc_check(!(r == MIB && ferror(f)), "fwrite() returned the full count with the error indicator set");
		if (r == MIB)
			pc_unresolved("the stream took in the whole MiB without writing");
		pc_check(ferror(f) != 0, "fwrite() returned %zu of %d but the error indicator is not set", r, MIB);
		pc_check(e == EPIPE, "errno is %s, expected EPIPE", pc_errname(e));
		NEED(sigpending(&s) == 0, "sigpending failed");
		pc_check(sigismember(&s, SIGPIPE) == 1, "SIGPIPE is not pending");
	}
	pc_pass();
}
