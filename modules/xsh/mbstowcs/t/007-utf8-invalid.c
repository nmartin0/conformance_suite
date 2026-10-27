/*
 * mbstowcs() returns (size_t)-1 with EILSEQ for an invalid byte and for a
 * character cut short by the end of the string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbstowcs.007
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <stdlib.h>

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
	static const char *const s[2] = { "a\x80", "a\xC3" };
	wchar_t w[8];
	size_t r;
	int e, i;

	pc_begin("XSH.mbstowcs.007");
	use_utf8();
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = EDOM;
		r = mbstowcs(w, s[i], 8);
		e = errno;
		pc_check(r == (size_t)-1, "mbstowcs() of string %d returned %zu, expected (size_t)-1", i, r);
		pc_check(e == EILSEQ, "mbstowcs() of string %d set errno to %s, expected EILSEQ", i, pc_errname(e));
	}
	pc_pass();
}
