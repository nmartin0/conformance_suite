/*
 * mbrlen() returns (size_t)-1 and sets errno to EILSEQ for an invalid
 * sequence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbrlen.005
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <string.h>
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
	size_t r;
	int e, i;

	pc_begin("XSH.mbrlen.005");
	use_utf8();
	pc_test();
	for (i = 0; i < 2; i++) {
		memset(&st, 0, sizeof st);
		errno = EDOM;
		r = mbrlen(s[i], n[i], &st);
		e = errno;
		pc_check(r == (size_t)-1, "mbrlen() of sequence %d returned %zu, expected (size_t)-1", i, r);
		pc_check(e == EILSEQ, "mbrlen() of sequence %d set errno to %s, expected EILSEQ", i, pc_errname(e));
	}
	pc_pass();
}
