/*
 * mbrtoc32() and mbrtoc16() return (size_t)-1, set errno to EILSEQ and
 * store nothing for an invalid sequence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrtoc16.008
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
	static const char *const s[2] = { "\x80", "\xC3\x28" };
	static const size_t n[2] = { 1, 2 };
	mbstate_t st;
	char32_t c32;
	char16_t c16;
	size_t r;
	int e, i;

	pc_begin("XSH.mbrtoc16.008");
	use_utf8();
	pc_test();
	for (i = 0; i < 2; i++) {
		memset(&st, 0, sizeof st);
		c32 = 77;
		errno = EDOM;
		r = mbrtoc32(&c32, s[i], n[i], &st);
		e = errno;
		pc_check(r == (size_t)-1, "mbrtoc32() of sequence %d returned %zu, expected (size_t)-1", i, r);
		pc_check(e == EILSEQ, "mbrtoc32() of sequence %d set errno to %s, expected EILSEQ", i, pc_errname(e));
		pc_check(c32 == 77, "mbrtoc32() stored a value for invalid sequence %d", i);
		memset(&st, 0, sizeof st);
		c16 = 77;
		errno = EDOM;
		r = mbrtoc16(&c16, s[i], n[i], &st);
		e = errno;
		pc_check(r == (size_t)-1, "mbrtoc16() of sequence %d returned %zu, expected (size_t)-1", i, r);
		pc_check(e == EILSEQ, "mbrtoc16() of sequence %d set errno to %s, expected EILSEQ", i, pc_errname(e));
		pc_check(c16 == 77, "mbrtoc16() stored a value for invalid sequence %d", i);
	}
	pc_pass();
}
