/*
 * mbtowc() returns -1 for an invalid sequence, with errno set to EILSEQ,
 * and returns -1 for one cut short by n.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbtowc.005
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
	static const char *const s[3] = { "\x80", "\xC3\x28", "\xC3\xA9" };
	static const size_t n[3] = { 1, 2, 1 };
	wchar_t wc;
	int r, e, i;

	pc_begin("XSH.mbtowc.005");
	use_utf8();
	pc_test();
	for (i = 0; i < 3; i++) {
		errno = EDOM;
		r = mbtowc(&wc, s[i], n[i]);
		e = errno;
		pc_check(r == -1, "mbtowc() of sequence %d returned %d, expected -1", i, r);
		if (i < 2)	/* the cut-short case is left open: see the .asr note */
			pc_check(e == EILSEQ, "mbtowc() of sequence %d set errno to %s, expected EILSEQ", i, pc_errname(e));
	}
	pc_pass();
}
