/*
 * putchar() fails with EBADF when descriptor 1 is closed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.putchar.004
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

/* Opens path with flags and makes it descriptor target (setup). */
static void fd_onto(const char *path, int flags, int target)
{
	int fd = open(path, flags, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	if (fd != target) {
		if (!(dup2(fd, target) == target))
			pc_unresolved("dup2 to %d failed: %s", target, pc_errname(errno));
		pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
	}
}

/*
 * Asks for an unbuffered stream (before any other operation on it): 1 if
 * setvbuf() honoured the request. Support for the buffering modes is
 * implementation-defined (XSH 2.5), so a refusal is not a failure.
 */
static int unbuffered(FILE *f)
{
	return setvbuf(f, NULL, _IONBF, 0) == 0;
}

/* Calls of a write function the error tests make, up to this many bytes. */
#define PUT_LIMIT (4L << 20)

/*
 * Writes with putchar() until a call fails and returns that call's errno.
 * On an unbuffered stream (unbuf) the first call must fail; on a buffered
 * one, a call fails once the buffer needs flushing, and if none does
 * within PUT_LIMIT bytes the test cannot judge.
 */
static int put_until_fail(int unbuf)
{
	long i, lim = unbuf ? 1 : PUT_LIMIT + 1;
	int r, e;

	for (i = 0; i < lim; i++) {
		errno = 0;
		r = putchar('x');
		e = errno;
		if (r == EOF)
			return e;
		pc_check(r == 'x', "the write returned %d", r);
	}
	if (unbuf)
		pc_fail("the write on the unbuffered stream succeeded: the error was not reported");
	pc_unresolved("no write failed within %ld bytes: the stream's buffer never needed flushing", PUT_LIMIT);
}

int main(void)
{
	pc_begin("XSH.putchar.004");
	{
		int nb, e;

		fd_onto("out", O_RDWR | O_CREAT | O_TRUNC, 1);
		nb = unbuffered(stdout);
		pc_test();
		NEED(close(1) == 0, "close(1) failed: %s", pc_errname(errno));
		e = put_until_fail(nb);
		pc_check(e == EBADF, "the failing putchar() set errno %s, expected EBADF", pc_errname(e));
		pc_check(ferror(stdout) != 0, "the error indicator of stdout is not set");
	}
	pc_pass();
}
