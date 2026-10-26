/*
 * puts() with standard output's descriptor closed fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.puts.002
 * MUTANT    puts.errno_lost
 * MUTANT    puts.success_on_error
 * VARIANT   setvbuf.refuses_modes
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

/* Puts a regular file path on descriptor 1, for stdout (setup). */
static void stdout_to(const char *path)
{
	int fd = open(path, O_RDWR | O_CREAT | O_TRUNC, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	if (!(dup2(fd, 1) == 1))
		pc_unresolved("dup2 failed: %s", pc_errname(errno));
	pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
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

static char ab[] = "ab";

/*
 * Writes with puts() until a call fails and returns that call's errno.
 * On an unbuffered stream (unbuf) the first call must fail; on a buffered
 * one, a call fails once the buffer needs flushing, and if none does
 * within PUT_LIMIT bytes the test cannot judge.
 */
static int put_until_fail(int unbuf)
{
	long i, lim = unbuf ? 1 : PUT_LIMIT / 3 + 1;
	int r, e;

	for (i = 0; i < lim; i++) {
		errno = 0;
		r = puts(ab);
		e = errno;
		if (r == EOF)
			return e;
		pc_check(r >= 0, "puts() returned %d", r);
	}
	if (unbuf)
		pc_fail("puts() on the unbuffered stream succeeded: the error was not reported");
	pc_unresolved("no puts() failed within %ld bytes: the stream's buffer never needed flushing", PUT_LIMIT);
}

int main(void)
{
	pc_begin("XSH.puts.002");
	{
		int nb, e;

		stdout_to("out");
		nb = unbuffered(stdout);
		pc_test();
		NEED(close(1) == 0, "close(1) failed: %s", pc_errname(errno));
		e = put_until_fail(nb);
		pc_check(e == EBADF, "the failing puts() set errno %s, expected EBADF", pc_errname(e));
		pc_check(ferror(stdout) != 0, "the error indicator of stdout is not set");
	}
	pc_pass();
}
