/*
 * wcrtomb() returns (size_t)-1 with EILSEQ for a code that is no character;
 * the POSIX locale has 256 characters, so among 257 distinct codes at least
 * one is invalid. At most MB_CUR_MAX bytes are stored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcrtomb.003
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	mbstate_t st;
	char buf[32];
	size_t max = MB_CUR_MAX, r;
	int v, e, nbad = 0, badv = -1, bade = 0, over = -1;

	pc_begin("XSH.wcrtomb.003");
	pc_check(max >= 1 && max < sizeof buf - 1, "setup: MB_CUR_MAX is %zu", max);
	pc_test();
	for (v = 0; v <= 256; v++) {
		memset(&st, 0, sizeof st);
		memset(buf, 'x', sizeof buf);
		errno = EDOM;
		r = wcrtomb(buf, (wchar_t)v, &st);
		e = errno;
		if (r == (size_t)-1) {
			nbad++;
			if (e != EILSEQ && bade == 0)
				bade = e;
		} else if (r != 1 && badv < 0)
			badv = v;
		if (over < 0 && buf[max] != 'x')
			over = v;
	}
	pc_check(badv < 0, "wcrtomb(%d) returned neither 1 nor (size_t)-1", badv);
	pc_check(bade == 0, "wcrtomb() failed with errno %s, expected EILSEQ", pc_errname(bade));
	pc_check(nbad > 0, "wcrtomb() accepted all 257 codes 0 to 256, but the locale has 256 characters");
	pc_check(over < 0, "wcrtomb(%d) stored more than MB_CUR_MAX bytes", over);
	pc_pass();
}
