/*
 * fputc() to a full non-blocking pipe fails with EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputc.009
 * MUTANT    fputc.errno_lost
 * MUTANT    fputc.success_on_error
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
 * Writes with fputc() until a call fails and returns that call's errno.
 * On an unbuffered stream (unbuf) the first call must fail; on a buffered
 * one, a call fails once the buffer needs flushing, and if none does
 * within PUT_LIMIT bytes the test cannot judge.
 */
static int put_until_fail(FILE *f, int unbuf)
{
	long i, lim = unbuf ? 1 : PUT_LIMIT / 1 + 1;
	int r, e;

	for (i = 0; i < lim; i++) {
		errno = 0;
		r = fputc('x', f);
		e = errno;
		if (r == EOF)
			return e;
		pc_check(r == 'x', "fputc() returned %d", r);
	}
	if (unbuf)
		pc_fail("fputc() on the unbuffered stream succeeded: the error was not reported");
	pc_unresolved("no fputc() failed within %ld bytes: the stream's buffer never needed flushing", PUT_LIMIT);
}

/* Fills the pipe completely (setup): non-blocking writes until EAGAIN; leaves O_NONBLOCK set. */
static void fill(int w)
{
	static char blk[4096];
	int fl = fcntl(w, F_GETFL);
	ssize_t r;

	pc_check(fl != -1 && fcntl(w, F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	while ((r = write(w, blk, sizeof blk)) > 0)
		;
	while ((r = write(w, blk, 1)) > 0)
		;
	pc_check(r == -1 && errno == EAGAIN, "could not fill the pipe");
}

int main(void)
{
	pc_begin("XSH.fputc.009");
	{
		FILE *f;
		int nb, e;
		int p[2];

		pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
		fill(p[1]);
		pc_test();
		f = fdopen(p[1], "w");
		NEED(f != NULL, "fdopen() failed: %s", pc_errname(errno));
		nb = unbuffered(f);

		e = put_until_fail(f, nb);
		pc_check(e == EAGAIN, "the failing fputc() set errno %s, expected EAGAIN", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
