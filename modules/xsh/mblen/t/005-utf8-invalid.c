/*
 * mblen() returns -1 for an invalid sequence; errno is left alone or set to
 * EILSEQ, the only error the page lists.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mblen.005
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
	static const char *const s[2] = { "\x80", "\xC3\x28" };
	static const size_t n[2] = { 1, 2 };
	int r, e, i;

	pc_begin("XSH.mblen.005");
	use_utf8();
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = EDOM;
		r = mblen(s[i], n[i]);
		e = errno;
		pc_check(r == -1, "mblen() of invalid sequence %d returned %d, expected -1", i, r);
		pc_check(e == EDOM || e == EILSEQ, "mblen() left errno at %s, expected it unchanged or EILSEQ", pc_errname(e));
	}
	pc_pass();
}
