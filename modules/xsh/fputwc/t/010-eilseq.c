/*
 * fputwc() of a code that is not a valid character fails with EILSEQ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fputwc.010
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

/* Setup: switches to an installed UTF-8 locale, or the test is not in use. */
static void utf8(void)
{
	const char *name = pc_find_locale("UTF-8", 1);

	if (name == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_ALL, name) == NULL)
		pc_unresolved("setlocale(%s) failed", name);
}


/* Setup: a wide-character code that wcrtomb() rejects, or the test is UNRESOLVED. */
static wchar_t bad_wc(void)
{
	static const unsigned long cand[] = { 0xd800UL, 0xdfffUL, 0x110000UL, 0x7fffffffUL, 0x80000000UL, 0xffffffffUL };
	size_t i;
	char buf[MB_LEN_MAX + 8];
	mbstate_t st;

	for (i = 0; i < sizeof cand / sizeof cand[0]; i++) {
		wchar_t w = (wchar_t)cand[i];

		if ((unsigned long)w != cand[i])
			continue;
		memset(&st, 0, sizeof st);
		errno = 0;
		if (wcrtomb(buf, w, &st) == (size_t)-1)
			return w;
	}
	pc_unresolved("no candidate wide-character code is rejected by wcrtomb()");
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

int main(void)
{
	pc_begin("XSH.fputwc.010");
	{
		FILE *f;
		wchar_t bad;
		wint_t r;
		int e;

		utf8();
		bad = bad_wc();
		pc_test();
		f = open_as("f", "w");
		NEED(setvbuf(f, NULL, _IONBF, 0) == 0, "setvbuf() refused an unbuffered stream");
		errno = 0;
		r = fputwc(bad, f);
		e = errno;
		pc_check(r == WEOF, "fputwc() of an invalid wide-character code returned %#lx, expected WEOF", (unsigned long)r);
		pc_check(e == EILSEQ, "fputwc() set errno %s, expected EILSEQ", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set after an encoding error");
	}
	pc_pass();
}
