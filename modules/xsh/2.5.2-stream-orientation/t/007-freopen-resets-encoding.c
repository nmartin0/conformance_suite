/*
 * freopen() clears the orientation and the encoding rule of a stream.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.007
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static wchar_t decode(const char *b, size_t n, int *ok)
{
	mbstate_t st;
	wchar_t w = 0;
	size_t r;

	memset(&st, 0, sizeof st);
	r = mbrtowc(&w, b, n, &st);
	*ok = (r == n);
	return w;
}

int main(void)
{
	const char *u8, *sb;
	char two[3] = { '\xc3', '\xa9', '\0' };
	wchar_t e, d1, d2;
	int ok, ok2;
	FILE *f;
	wint_t a, b, c;

	pc_begin("XSH.2.5.2-stream-orientation.007");
	u8 = pc_find_locale("UTF-8", 1);
	if (u8 == NULL)
		pc_notinuse("no installed locale with a multibyte UTF-8 code set");
	sb = pc_find_locale("ISO-8859-1", 0);
	if (sb == NULL)
		sb = "C";
	NEED(setlocale(LC_CTYPE, u8) != NULL, "setlocale(%s) failed", u8);
	e = decode(two, 2, &ok);
	NEED(ok, "the UTF-8 locale does not decode the two bytes as one character");
	NEED(setlocale(LC_CTYPE, sb) != NULL, "setlocale(%s) failed", sb);
	NEED(MB_CUR_MAX == 1, "the second locale is not a single-byte one");
	d1 = decode(two, 1, &ok);
	d2 = decode(two + 1, 1, &ok2);
	NEED(ok && ok2, "the single-byte locale does not decode the bytes (%s)", sb);
	NEED(d1 != e, "the two locales decode alike; the test cannot tell them apart");
	pc_make_file("f", "\xc3\xa9");
	f = fopen("f", "r");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	NEED(setlocale(LC_CTYPE, u8) != NULL, "setlocale(%s) failed", u8);
	NEED(fwide(f, 1) > 0, "fwide(f, 1) did not make the stream wide-oriented");
	NEED(setlocale(LC_CTYPE, sb) != NULL, "setlocale(%s) failed", sb);
	pc_test();
	pc_check(freopen("f", "r", f) == f, "freopen failed: %s", pc_errname(errno));
	pc_check(fwide(f, 0) == 0, "after freopen() the stream still has an orientation");
	pc_check(fwide(f, 1) > 0, "fwide(f, 1) after freopen() did not give wide orientation");
	a = fgetwc(f);
	b = fgetwc(f);
	c = fgetwc(f);
	pc_check(a == (wint_t)d1 && b == (wint_t)d2 && c == WEOF,
	    "after freopen() the stream did not use the encoding of the locale in force "
	    "when it became wide-oriented again (read %lu %lu)", (unsigned long)a, (unsigned long)b);
	pc_pass();
}
