/*
 * fputs() on a stream whose descriptor is closed fails with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputs.002
 * MUTANT    fputs.errno_lost
 * MUTANT    fputs.success_on_error
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

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
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
 * Writes with fputs() until a call fails and returns that call's errno.
 * On an unbuffered stream (unbuf) the first call must fail; on a buffered
 * one, a call fails once the buffer needs flushing, and if none does
 * within PUT_LIMIT bytes the test cannot judge.
 */
static int put_until_fail(FILE *f, int unbuf)
{
	long i, lim = unbuf ? 1 : PUT_LIMIT / 2 + 1;
	int r, e;

	for (i = 0; i < lim; i++) {
		errno = 0;
		r = fputs(ab, f);
		e = errno;
		if (r == EOF)
			return e;
		pc_check(r >= 0, "fputs() returned %d", r);
	}
	if (unbuf)
		pc_fail("fputs() on the unbuffered stream succeeded: the error was not reported");
	pc_unresolved("no fputs() failed within %ld bytes: the stream's buffer never needed flushing", PUT_LIMIT);
}

int main(void)
{
	pc_begin("XSH.fputs.002");
	{
		FILE *f;
		int nb, e;

		pc_test();
		f = open_as("f", "w");
		nb = unbuffered(f);
		NEED(close(fileno(f)) == 0, "close() of the stream's descriptor failed: %s", pc_errname(errno));
		e = put_until_fail(f, nb);
		pc_check(e == EBADF, "the failing fputs() set errno %s, expected EBADF", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
