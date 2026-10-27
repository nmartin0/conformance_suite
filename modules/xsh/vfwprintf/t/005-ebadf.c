/*
 * vfwprintf() and vwprintf() on a closed descriptor fail with EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfwprintf.005
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

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

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

static int vsw(wchar_t *s, size_t n, const wchar_t *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vswprintf(s, n, fmt, ap);
	va_end(ap);
	return r;
}

static int vfw(FILE *f, const wchar_t *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vfwprintf(f, fmt, ap);
	va_end(ap);
	return r;
}

static int vw(const wchar_t *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vwprintf(fmt, ap);
	va_end(ap);
	return r;
}

#define CALL_S(b, ...) vsw(b, 64, __VA_ARGS__)
#define CALL_F(g, ...) vfw(g, __VA_ARGS__)
#define CALL_W(...) vw(__VA_ARGS__)

static int op_vfw(FILE *f)
{
	return vfw(f, L"x") < 0;
}

static int op_vw(FILE *f)
{
	(void)f;
	return vw(L"x") < 0;
}

int main(void)
{
	pc_begin("XSH.vfwprintf.005");
	{
		FILE *f;
		int nb, nb2, e;

		fd_onto("out", O_RDWR | O_CREAT | O_TRUNC, 1);
		nb2 = unbuffered(stdout);
		pc_test();
		f = open_as("f", "w");
		nb = unbuffered(f);
		NEED(close(fileno(f)) == 0, "close() of the stream's descriptor failed: %s", pc_errname(errno));
		e = put_until_fail(f, nb, op_vfw);
		pc_check(e == EBADF, "the failing vfwprintf() set errno %s, expected EBADF", pc_errname(e));
		NEED(close(1) == 0, "close(1) failed: %s", pc_errname(errno));
		e = put_until_fail(stdout, nb2, op_vw);
		pc_check(e == EBADF, "the failing vwprintf() set errno %s, expected EBADF", pc_errname(e));
	}
	pc_pass();
}
