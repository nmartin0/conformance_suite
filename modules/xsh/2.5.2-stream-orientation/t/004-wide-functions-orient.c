/*
 * Each wide-character input/output function makes a new stream wide-oriented.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.004
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <wchar.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

enum { FPUTWC, FPUTWS, FWPRINTF, PUTWC, FGETWC, FGETWS, FWSCANF, GETWC, NOPS };
static const char *const opname[NOPS] = {
	"fputwc", "fputws", "fwprintf", "putwc", "fgetwc", "fgetws", "fwscanf", "getwc"
};

int main(void)
{
	FILE *s[NOPS];
	int i;

	pc_begin("XSH.2.5.2-stream-orientation.004");
	pc_make_file("in", "abcdef");
	for (i = 0; i < NOPS; i++) {
		s[i] = fopen(i < FGETWC ? "out" : "in", i < FGETWC ? "w" : "r");
		NEED(s[i] != NULL, "fopen failed: %s", pc_errname(errno));
		NEED(fwide(s[i], 0) == 0, "a new stream is not without orientation");
	}
	pc_test();
	{
		wchar_t buf[8], ch;
		int rc[NOPS];

		rc[FPUTWC] = fputwc(L'x', s[FPUTWC]) == L'x';
		rc[FPUTWS] = fputws(L"x", s[FPUTWS]) >= 0;
		rc[FWPRINTF] = fwprintf(s[FWPRINTF], L"x") == 1;
		rc[PUTWC] = putwc(L'x', s[PUTWC]) == L'x';
		rc[FGETWC] = fgetwc(s[FGETWC]) == L'a';
		rc[FGETWS] = fgetws(buf, 8, s[FGETWS]) != NULL;
		rc[FWSCANF] = fwscanf(s[FWSCANF], L"%lc", &ch) == 1;
		rc[GETWC] = getwc(s[GETWC]) == L'a';
		for (i = 0; i < NOPS; i++) {
			int o;

			NEED(rc[i], "%s did not succeed", opname[i]);
			o = fwide(s[i], 0);
			pc_check(o > 0, "after %s fwide() returned %d, expected a positive value", opname[i], o);
		}
	}
	pc_pass();
}
