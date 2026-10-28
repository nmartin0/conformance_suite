/*
 * Wide-character input gives what mbrtowc() makes of the bytes of the file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.008
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

static unsigned char bytes[MAXC * 4];
static size_t nbytes;
static wchar_t expect[MAXC];
static size_t nexp;

/* Decode bytes[0..nbytes) as mbrtowc() does, one byte at a time. */
static void decode_all(void)
{
	mbstate_t st;
	size_t i;

	memset(&st, 0, sizeof st);
	nexp = 0;
	for (i = 0; i < nbytes; i++) {
		wchar_t w = 0;
		char c = (char)bytes[i];
		size_t r = mbrtowc(&w, &c, 1, &st);

		if (r == (size_t)-2)
			continue;
		NEED(r != (size_t)-1, "the bytes made for the test do not decode");
		NEED(nexp < MAXC, "too many characters");
		expect[nexp++] = w;
	}
}

static void write_file(void)
{
	FILE *f = fopen("in", "wb");

	NEED(f != NULL && fwrite(bytes, 1, nbytes, f) == nbytes && fclose(f) == 0, "cannot write the input file");
}

static void check_readers(const char *where)
{
	FILE *f;
	size_t i;
	wint_t c;
	static wchar_t line[MAXC + 2];

	/* fgetwc */
	f = fopen("in", "r");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	for (i = 0; i < nexp; i++) {
		c = fgetwc(f);
		pc_check(c == (wint_t)expect[i], "%s: fgetwc() character %lu is %lu, mbrtowc() gives %lu", where,
		    (unsigned long)i, (unsigned long)c, (unsigned long)expect[i]);
	}
	pc_check(fgetwc(f) == WEOF && feof(f), "%s: fgetwc() did not end the file after %lu characters", where,
	    (unsigned long)nexp);
	(void)fclose(f);
	/* getwc */
	f = fopen("in", "r");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	for (i = 0; i < nexp; i++) {
		c = getwc(f);
		pc_check(c == (wint_t)expect[i], "%s: getwc() character %lu is %lu, mbrtowc() gives %lu", where,
		    (unsigned long)i, (unsigned long)c, (unsigned long)expect[i]);
	}
	(void)fclose(f);
	/* fgetws: the characters, up to the first new-line or the end; none of them is a new-line or null here */
	f = fopen("in", "r");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	NEED(fgetws(line, (int)nexp + 2, f) == line, "fgetws returned a null pointer");
	pc_check(wcslen(line) == nexp, "%s: fgetws() returned %lu characters, expected %lu", where,
	    (unsigned long)wcslen(line), (unsigned long)nexp);
	for (i = 0; i < nexp; i++)
		pc_check(line[i] == expect[i], "%s: fgetws() character %lu is %lu, mbrtowc() gives %lu", where,
		    (unsigned long)i, (unsigned long)line[i], (unsigned long)expect[i]);
	(void)fclose(f);
}

int main(void)
{
	const char *u8;
	int b;

	pc_begin("XSH.2.5.2-stream-orientation.008");
	u8 = pc_find_locale("UTF-8", 1);
	pc_test();

	/* The POSIX locale: every byte value that mbrtowc() accepts, except the null byte and the new-line. */
	NEED(setlocale(LC_ALL, "C") != NULL, "setlocale(C) failed");
	{
		mbstate_t st;

		nbytes = 0;
		for (b = 1; b < 256; b++) {
			wchar_t w;
			char c = (char)b;
			size_t r;

			if (b == '\n')
				continue;
			memset(&st, 0, sizeof st);
			r = mbrtowc(&w, &c, 1, &st);
			if (r == 1 && w != L'\0' && w != L'\n')
				bytes[nbytes++] = (unsigned char)b;
		}
	}
	NEED(nbytes >= 64, "the POSIX locale accepts fewer than 64 single bytes");
	decode_all();
	write_file();
	check_readers("POSIX locale");

	if (u8 != NULL) {
		static const unsigned char text[] = {
			'a', 0xc3, 0xa9, 'b', 0xe2, 0x82, 0xac, 'c', 0xf0, 0x90, 0x8d, 0x88, 'd', 0xc2, 0x80,
			0xdf, 0xbf, 0xe0, 0xa0, 0x80, 'z'
		};

		NEED(setlocale(LC_ALL, u8) != NULL, "setlocale(%s) failed", u8);
		memcpy(bytes, text, sizeof text);
		nbytes = sizeof text;
		decode_all();
		write_file();
		check_readers("UTF-8 locale");
	}
	pc_pass();
}
