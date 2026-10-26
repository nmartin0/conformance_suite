/*
 * %C and %S convert as %lc and %ls.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.040
 * MUTANT    fscanf.xsi_cs_unknown
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
	wchar_t w[8], wa, wb, wx, wy;
	int r;
	size_t i;

	pc_begin("XSH.fscanf.040");
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	wa = wide_of('a');
	wb = wide_of('b');
	wx = wide_of('x');
	wy = wide_of('y');
	pc_test();
	for (i = 0; i < 8; i++)
		w[i] = 0x5a5a;
	r = sscanf("xyz", "%2C", w);
	pc_check(r == 1 && w[0] == wx && w[1] == wy && w[2] == 0x5a5a,
	    "\"%%2C\" on \"xyz\" returned %d and not two wide characters 'x' 'y'", r);
	for (i = 0; i < 8; i++)
		w[i] = 0x5a5a;
	r = sscanf("ab cd", "%S", w);
	pc_check(r == 1 && w[0] == wa && w[1] == wb && w[2] == 0, "\"%%S\" on \"ab cd\" returned %d and not the wide \"ab\"", r);
	pc_pass();
}
