/*
 * Wide-character output writes what wcrtomb() makes of the characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.009
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)
#define MAXC 600

static wchar_t chars[MAXC + 1];
static size_t nchars;
static unsigned char want[MAXC * 8];
static size_t nwant;

/* What wcrtomb() makes of chars[0..nchars), stored in want. */
static void encode_all(void)
{
	mbstate_t st;
	size_t i;

	memset(&st, 0, sizeof st);
	nwant = 0;
	for (i = 0; i < nchars; i++) {
		char out[16];
		size_t r = wcrtomb(out, chars[i], &st);

		NEED(r != (size_t)-1 && r < sizeof out, "a test character does not encode");
		memcpy(want + nwant, out, r);
		nwant += r;
	}
	chars[nchars] = L'\0';
}

static int file_is_want(const char *path)
{
	unsigned char got[MAXC * 8 + 1];
	FILE *f = fopen(path, "rb");
	size_t n;

	NEED(f != NULL, "cannot read %s back", path);
	n = fread(got, 1, sizeof got, f);
	(void)fclose(f);
	return n == nwant && memcmp(got, want, n) == 0;
}

static void check_writers(const char *where)
{
	FILE *f;
	size_t i;

	f = fopen("o1", "w");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	for (i = 0; i < nchars; i++)
		pc_check(fputwc(chars[i], f) == (wint_t)chars[i], "%s: fputwc() failed on character %lu", where,
		    (unsigned long)i);
	pc_check(fclose(f) == 0, "%s: fclose failed", where);
	pc_check(file_is_want("o1"), "%s: fputwc() wrote bytes other than those of wcrtomb()", where);

	f = fopen("o2", "w");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	for (i = 0; i < nchars; i++)
		pc_check(putwc(chars[i], f) == (wint_t)chars[i], "%s: putwc() failed on character %lu", where,
		    (unsigned long)i);
	pc_check(fclose(f) == 0, "%s: fclose failed", where);
	pc_check(file_is_want("o2"), "%s: putwc() wrote bytes other than those of wcrtomb()", where);

	f = fopen("o3", "w");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	pc_check(fputws(chars, f) >= 0, "%s: fputws() failed", where);
	pc_check(fclose(f) == 0, "%s: fclose failed", where);
	pc_check(file_is_want("o3"), "%s: fputws() wrote bytes other than those of wcrtomb()", where);
}

int main(void)
{
	const char *u8;
	int b;

	pc_begin("XSH.2.5.2-stream-orientation.009");
	u8 = pc_find_locale("UTF-8", 1);
	pc_test();

	NEED(setlocale(LC_ALL, "C") != NULL, "setlocale(C) failed");
	nchars = 0;
	for (b = 1; b < 256; b++) {
		char out[16];
		mbstate_t st;
		wchar_t w;
		mbstate_t st2;
		char c = (char)b;

		/* Codes come from decoding bytes, so that only characters of the locale are used. */
		memset(&st, 0, sizeof st);
		if (mbrtowc(&w, &c, 1, &st) != 1 || w == L'\0')
			continue;
		memset(&st2, 0, sizeof st2);
		if (wcrtomb(out, w, &st2) == (size_t)-1)
			continue;
		chars[nchars++] = w;
	}
	NEED(nchars >= 64, "the POSIX locale has fewer than 64 usable characters");
	encode_all();
	check_writers("POSIX locale");

	if (u8 != NULL) {
		static const unsigned char text[] = {
			'a', 0xc3, 0xa9, 'b', 0xe2, 0x82, 0xac, 'c', 0xf0, 0x90, 0x8d, 0x88, 'd', 0xc2, 0x80,
			0xdf, 0xbf, 0xe0, 0xa0, 0x80, 'z'
		};
		size_t i;
		mbstate_t st;

		NEED(setlocale(LC_ALL, u8) != NULL, "setlocale(%s) failed", u8);
		memset(&st, 0, sizeof st);
		nchars = 0;
		for (i = 0; i < sizeof text; i++) {
			wchar_t w = 0;
			char c = (char)text[i];
			size_t r = mbrtowc(&w, &c, 1, &st);

			if (r == (size_t)-2)
				continue;
			NEED(r == 1, "the UTF-8 test text does not decode");
			chars[nchars++] = w;
		}
		encode_all();
		check_writers("UTF-8 locale");
	}
	pc_pass();
}
