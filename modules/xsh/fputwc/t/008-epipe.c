/*
 * fputwc() on a pipe with no reader fails with EPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputwc.008
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

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

static void block_sigpipe(void)
{
	sigset_t one;

	sigemptyset(&one);
	sigaddset(&one, SIGPIPE);
	if (!(sigprocmask(SIG_BLOCK, &one, NULL) == 0))
		pc_unresolved("sigprocmask failed");
}

/* Calls of a write function the error tests make, up to this many. */
#define PUT_LIMIT (4L << 20)

/*
 * Calls op() until one fails and returns that call's errno. On an
 * unbuffered stream (unbuf) the first call must fail; on a buffered one,
 * a call fails once the buffer needs flushing, and if none does within
 * PUT_LIMIT calls the test cannot judge.
 */
static int put_until_fail(FILE *f, int unbuf, int (*op)(FILE *))
{
	long i, lim = unbuf ? 1 : PUT_LIMIT + 1;
	int e;

	for (i = 0; i < lim; i++) {
		errno = 0;
		if (op(f) != 0) {
			e = errno;
			return e;
		}
	}
	if (unbuf)
		pc_fail("the call on the unbuffered stream succeeded: the error was not reported");
	pc_unresolved("no call failed within %ld: the stream's buffer never needed flushing", PUT_LIMIT);
}

static int op_fputwc(FILE *f)
{
	return fputwc(L'x', f) == WEOF;
}

int main(void)
{
	pc_begin("XSH.fputwc.008");
	{
		FILE *f;
		int nb, e, p[2];

		block_sigpipe();
		pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
		(void)close(p[0]);
		pc_test();
		f = fdopen(p[1], "w");
		NEED(f != NULL, "fdopen() failed: %s", pc_errname(errno));
		nb = unbuffered(f);
		e = put_until_fail(f, nb, op_fputwc);
		pc_check(e == EPIPE, "the failing fputwc() set errno %s, expected EPIPE", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
