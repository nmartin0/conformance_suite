/*
 * In a UTF-8 locale c32rtomb() and c16rtomb() return (size_t)-1 with EILSEQ
 * for codes that are no Unicode character: a surrogate or a value past
 * U+10FFFF as a 32-bit code, a low surrogate alone as a 16-bit code.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.c16rtomb.008
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <string.h>
#include <uchar.h>
#include <wchar.h>

static void use_utf8(void)
{
	const char *n = pc_find_locale("UTF-8", 1);

	if (n == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_ALL, n) == NULL)
		pc_unresolved("setlocale(LC_ALL, \"%s\") failed", n);
}

int main(void)
{
#if !defined(__STDC_UTF_32__) || !defined(__STDC_UTF_16__)
	pc_begin("XSH.c16rtomb.008");
	pc_notinuse("char16_t and char32_t are not announced as UTF-16 and UTF-32");
#else
	static const char32_t c32[2] = { 0xD800, 0x110000 };
	mbstate_t st;
	char buf[16];
	size_t r;
	int e, i;

	pc_begin("XSH.c16rtomb.008");
	use_utf8();
	pc_test();
	for (i = 0; i < 2; i++) {
		memset(&st, 0, sizeof st);
		errno = EDOM;
		r = c32rtomb(buf, c32[i], &st);
		e = errno;
		pc_check(r == (size_t)-1, "c32rtomb() of 0x%lX returned %zu, expected (size_t)-1", (unsigned long)c32[i], r);
		pc_check(e == EILSEQ, "c32rtomb() of 0x%lX set errno to %s, expected EILSEQ", (unsigned long)c32[i], pc_errname(e));
	}
	memset(&st, 0, sizeof st);
	errno = EDOM;
	r = c16rtomb(buf, 0xDC00, &st);
	e = errno;
	pc_check(r == (size_t)-1, "c16rtomb() of a lone low surrogate returned %zu, expected (size_t)-1", r);
	pc_check(e == EILSEQ, "c16rtomb() of a lone low surrogate set errno to %s, expected EILSEQ", pc_errname(e));
	pc_pass();
#endif
}
