/*
 * fputc() to a pipe with no reader fails with EPIPE and sends SIGPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputc.008
 * MUTANT    fputc.errno_lost
 * MUTANT    fputc.success_on_error
 * MUTANT    fputc.no_sigpipe
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

static void block_sigpipe(void)
{
	sigset_t one;

	sigemptyset(&one);
	sigaddset(&one, SIGPIPE);
	pc_check(sigprocmask(SIG_BLOCK, &one, NULL) == 0, "sigprocmask failed");
}

static int sigpipe_pending(void)
{
	sigset_t p;

	return sigpending(&p) == 0 && sigismember(&p, SIGPIPE) == 1;
}

int main(void)
{
	pc_begin("XSH.fputc.008");
	{
		FILE *f;
		int nb, e;
		int p[2];

		block_sigpipe();
		pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
		(void)close(p[0]);
		pc_test();
		f = fdopen(p[1], "w");
		NEED(f != NULL, "fdopen() failed: %s", pc_errname(errno));
		nb = unbuffered(f);

		e = put_until_fail(f, nb);
		pc_check(e == EPIPE, "the failing fputc() set errno %s, expected EPIPE", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
		pc_check(sigpipe_pending(), "no SIGPIPE is pending");
	}
	pc_pass();
}
