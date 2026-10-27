/*
 * fwprintf() fails with EILSEQ on a code that is not a valid character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwprintf.018
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

int main(void)
{
	pc_begin("XSH.fwprintf.018");
	{
		wchar_t ws[4], buf[8];
		FILE *f, *g, *h;
		int r, e;

		utf8();
		ws[0] = L'a';
		ws[1] = bad_wc();
		ws[2] = L'b';
		ws[3] = L'\0';
		pc_test();
		f = open_as("f", "w");
		errno = 0;
		r = fwprintf(f, L"%lc", (wint_t)ws[1]);
		e = errno;
		pc_check(r < 0, "fwprintf(%%lc) of an invalid wide-character code returned %d, expected a negative value", r);
		pc_check(e == EILSEQ, "fwprintf(%%lc) set errno %s, expected EILSEQ", pc_errname(e));
		g = open_as("g", "w");
		errno = 0;
		r = fwprintf(g, L"%ls", ws);
		e = errno;
		pc_check(r < 0, "fwprintf(%%ls) of a string holding an invalid wide-character code returned %d, expected a negative value", r);
		pc_check(e == EILSEQ, "fwprintf(%%ls) set errno %s, expected EILSEQ", pc_errname(e));
		/* 0xff is never a valid character in UTF-8: the %s conversion meets an encoding error. */
		wmemset(buf, L'#', 8);
		r = swprintf(buf, 8, L"%s", "\xff");
		pc_check(r < 0, "swprintf(%%s) of the invalid byte 0xff returned %d, expected a negative value", r);
		h = open_as("h", "w");
		errno = 0;
		r = fwprintf(h, L"%s", "\xff");
		e = errno;
		pc_check(r < 0, "fwprintf(%%s) of the invalid byte 0xff returned %d, expected a negative value", r);
		pc_check(e == EILSEQ, "fwprintf(%%s) of the invalid byte 0xff set errno %s, expected EILSEQ", pc_errname(e));
	}
	pc_pass();
}
