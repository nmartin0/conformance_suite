/*
 * fgetpos() and fsetpos() on a wide-oriented stream restore position and parse state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.005
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	const char *u8;
	FILE *f;
	fpos_t p, q;
	wint_t first[8], again[8];
	int n, m, k;

	pc_begin("XSH.2.5.2-stream-orientation.005");
	u8 = pc_find_locale("UTF-8", 1);
	if (u8 == NULL)
		pc_notinuse("no installed locale with a multibyte UTF-8 code set");
	NEED(setlocale(LC_ALL, u8) != NULL, "setlocale(%s) failed", u8);
	pc_make_file("f", "a" "\xc3\xa9" "b" "\xe2\x82\xac" "c" "\xf0\x90\x8d\x88" "d");
	f = fopen("f", "r");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	NEED(fwide(f, 1) > 0, "fwide(f, 1) did not make the stream wide-oriented");
	pc_test();
	pc_check(fgetwc(f) != WEOF, "fgetwc failed on the first character");
	pc_check(fgetwc(f) != WEOF, "fgetwc failed on the second character");
	pc_check(fgetpos(f, &p) == 0, "fgetpos failed");
	for (n = 0; n < 8; n++) {
		first[n] = fgetwc(f);
		if (first[n] == WEOF)
			break;
	}
	pc_check(n == 5, "after the two characters %d characters were read, expected 5", n);
	pc_check(fgetpos(f, &q) == 0, "fgetpos at the end failed");
	pc_check(fsetpos(f, &p) == 0, "fsetpos failed");
	pc_check(feof(f) == 0, "fsetpos left the end-of-file indicator set");
	for (m = 0; m < 8; m++) {
		again[m] = fgetwc(f);
		if (again[m] == WEOF)
			break;
	}
	pc_check(m == n, "after fsetpos %d characters were read, expected %d", m, n);
	for (k = 0; k < n; k++)
		pc_check(first[k] == again[k], "character %d differs after fsetpos", k);
	pc_check(fsetpos(f, &q) == 0, "fsetpos to the end position failed");
	pc_check(fgetwc(f) == WEOF, "a character was read from the saved end position");
	pc_pass();
}
