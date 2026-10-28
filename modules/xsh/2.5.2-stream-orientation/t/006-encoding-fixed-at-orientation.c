/*
 * The encoding rule of a stream is the one in force when it becomes wide-oriented.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.006
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* The wide character the two bytes make in the current locale, as one character. */
static wchar_t decode2(const char *b, int *ok)
{
	mbstate_t st;
	wchar_t w = 0;
	size_t r;

	memset(&st, 0, sizeof st);
	r = mbrtowc(&w, b, 2, &st);
	*ok = (r == 2);
	return w;
}

/* The wide character one byte makes in the current locale. */
static wchar_t decode1(const char *b, int *ok)
{
	mbstate_t st;
	wchar_t w = 0;
	size_t r;

	memset(&st, 0, sizeof st);
	r = mbrtowc(&w, b, 1, &st);
	*ok = (r == 1);
	return w;
}

int main(void)
{
	const char *u8, *sb;
	char two[3] = { '\xc3', '\xa9', '\0' };
	wchar_t e, d;
	int ok1, ok2;
	FILE *f;
	wint_t a, b;

	pc_begin("XSH.2.5.2-stream-orientation.006");
	u8 = pc_find_locale("UTF-8", 1);
	if (u8 == NULL)
		pc_notinuse("no installed locale with a multibyte UTF-8 code set");
	sb = pc_find_locale("ISO-8859-1", 0);
	if (sb == NULL)
		sb = "C";
	NEED(setlocale(LC_CTYPE, u8) != NULL, "setlocale(%s) failed", u8);
	e = decode2(two, &ok1);
	NEED(ok1, "the UTF-8 locale does not decode the two bytes as one character");
	NEED(setlocale(LC_CTYPE, sb) != NULL, "setlocale(%s) failed", sb);
	NEED(MB_CUR_MAX == 1, "the second locale is not a single-byte one");
	d = decode1(two, &ok1);
	(void)decode1(two + 1, &ok2);
	NEED(ok1 && ok2, "the single-byte locale does not decode the bytes (%s)", sb);
	NEED(d != e, "the two locales decode alike; the test cannot tell them apart");
	pc_make_file("f", "\xc3\xa9");
	f = fopen("f", "r");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	NEED(setlocale(LC_CTYPE, u8) != NULL, "setlocale(%s) failed", u8);
	NEED(fwide(f, 1) > 0, "fwide(f, 1) did not make the stream wide-oriented");
	NEED(setlocale(LC_CTYPE, sb) != NULL, "setlocale(%s) failed", sb);
	pc_test();
	a = fgetwc(f);
	pc_check(a == (wint_t)e, "the first character read is %lu, not the one character (%lu) of the encoding "
	    "in force at orientation", (unsigned long)a, (unsigned long)e);
	b = fgetwc(f);
	pc_check(b == WEOF && feof(f), "a second character was read: the stream followed the new locale");
	pc_pass();
}
