/*
 * 'm' with ls and l[ allocates a wide-character result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.038
 * MUTANT    fscanf.ml_narrow
 * MUTANT    fscanf.m_not_malloc
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* The wide character mbrtowc() makes of the single byte c (setup). */
static wchar_t wide_of(char c)
{
	mbstate_t st;
	wchar_t wc = 0;
	size_t n;

	memset(&st, 0, sizeof st);
	n = mbrtowc(&wc, &c, 1, &st);
	pc_check(n == 1, "mbrtowc() of %#x returned %ld", (unsigned)(unsigned char)c, (long)n);
	return wc;
}

int main(void)
{
	wchar_t *p = NULL, *q = NULL, wa, wb, wc, wd;
	int r;

	pc_begin("XSH.fscanf.038");
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	wa = wide_of('a');
	wb = wide_of('b');
	wc = wide_of('c');
	wd = wide_of('d');
	pc_test();
	r = sscanf("ab cd", "%mls %ml[cd]", &p, &q);
	pc_check(r == 2, "sscanf() returned %d, expected 2", r);
	pc_check(p != NULL && p[0] == wa && p[1] == wb && p[2] == 0, "%%mls did not give the wide string of \"ab\"");
	pc_check(q != NULL && q[0] == wc && q[1] == wd && q[2] == 0, "%%ml[cd] did not give the wide string of \"cd\"");
	free(p);
	free(q);
	pc_pass();
}
