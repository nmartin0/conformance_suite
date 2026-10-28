/*
 * errno is EILSEQ after an encoding error and is not made EILSEQ by valid input or output.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.010
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* One call, made with errno set to EDOM; its result must be right and errno not EILSEQ. */
#define CLEAN(name, ok) do { \
	int ok_, e_; \
	errno = EDOM; \
	ok_ = (ok) ? 1 : 0; \
	e_ = errno; \
	pc_check(ok_, "%s did not succeed", name); \
	pc_check(e_ != EILSEQ, "%s stored EILSEQ although no encoding error occurred", name); \
} while (0)

int main(void)
{
	const char *u8;
	FILE *bo, *bi, *wo, *wi, *so, *si;
	wchar_t wbuf[8];
	char cbuf[8];
	static const wchar_t cand[] = { 0x110000, 0x7fffffff, 0xd800, 0x200000, 0x3fffffff };
	size_t i;
	wchar_t bad = 0;
	char junk[8];

	pc_begin("XSH.2.5.2-stream-orientation.010");
	u8 = pc_find_locale("UTF-8", 1);
	pc_make_file("in", "abcdef\nghijkl\n");
	bo = fopen("o1", "w");
	bi = fopen("in", "r");
	wo = fopen("o2", "w");
	wi = fopen("in", "r");
	so = fopen("o3", "w");
	si = fopen("in", "r");
	NEED(bo && bi && wo && wi && so && si, "fopen failed: %s", pc_errname(errno));
	NEED(setlocale(LC_ALL, "C") != NULL, "setlocale(C) failed");
	pc_test();
	CLEAN("fputc", fputc('a', bo) == 'a');
	CLEAN("fgetc", fgetc(bi) == 'a');
	CLEAN("fputwc", fputwc(L'a', wo) == L'a');
	CLEAN("fgetwc", fgetwc(wi) == L'a');
	CLEAN("fputws", fputws(L"bc", so) >= 0);
	CLEAN("fgetws", fgetws(wbuf, 8, si) != NULL);
	CLEAN("fgets", fgets(cbuf, 8, bi) != NULL);
	CLEAN("fputs", fputs("bc", bo) >= 0);
	(void)fclose(bo); (void)fclose(bi); (void)fclose(wo); (void)fclose(wi); (void)fclose(so); (void)fclose(si);

	if (u8 == NULL)
		pc_notinuse("no UTF-8 locale: the encoding-error half of the assertion cannot be tried "
		    "(the half without an error passed)");
	NEED(setlocale(LC_ALL, u8) != NULL, "setlocale(%s) failed", u8);
	{
		FILE *f;
		wint_t c;

		memcpy(junk, "a\xff" "b", 4);
		{
			FILE *g = fopen("junk", "wb");

			NEED(g != NULL && fwrite(junk, 1, 3, g) == 3 && fclose(g) == 0, "cannot make the input file");
		}
		f = fopen("junk", "r");
		NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
		NEED(fgetwc(f) == L'a', "valid input was not read");
		errno = 0;
		c = fgetwc(f);
		pc_check(c == WEOF, "fgetwc() of the byte 0xff returned a character");
		pc_check(errno == EILSEQ, "fgetwc() of the byte 0xff set errno to %s, not EILSEQ", pc_errname(errno));
		(void)fclose(f);

		for (i = 0; i < sizeof cand / sizeof cand[0]; i++) {
			char out[16];
			mbstate_t st;

			memset(&st, 0, sizeof st);
			if (wcrtomb(out, cand[i], &st) == (size_t)-1) {
				bad = cand[i];
				break;
			}
		}
		if (bad == 0)
			pc_notinuse("every candidate code is a character of the locale: no encoding error on output");
		f = fopen("o4", "w");
		NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
		NEED(setvbuf(f, NULL, _IONBF, 0) == 0, "setvbuf failed");
		errno = 0;
		c = fputwc(bad, f);
		pc_check(c == WEOF, "fputwc() of a code wcrtomb() rejects returned a character");
		pc_check(errno == EILSEQ, "fputwc() of an unencodable code set errno to %s, not EILSEQ", pc_errname(errno));
		(void)fclose(f);
	}
	pc_pass();
}
