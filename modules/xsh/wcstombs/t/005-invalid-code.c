/*
 * wcstombs() returns (size_t)-1 with EILSEQ for a code that is no
 * character; the POSIX locale has 256 characters, so among the codes 1 to
 * 257 at least one is invalid.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcstombs.005
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>

int main(void)
{
	wchar_t one[2] = { 0, 0 }, mid[4];
	char buf[16];
	size_t r;
	int v, e, found = -1, badv = -1, bade = 0;

	pc_begin("XSH.wcstombs.005");
	pc_test();
	for (v = 1; v <= 257; v++) {
		one[0] = (wchar_t)v;
		errno = EDOM;
		r = wcstombs(buf, one, sizeof buf);
		e = errno;
		if (r == (size_t)-1) {
			if (found < 0)
				found = v;
			if (e != EILSEQ && bade == 0)
				bade = e;
		} else if (r != 1 && badv < 0)
			badv = v;
	}
	pc_check(badv < 0, "wcstombs() of the single code %d returned neither 1 nor (size_t)-1", badv);
	pc_check(bade == 0, "wcstombs() failed with errno %s, expected EILSEQ", pc_errname(bade));
	pc_check(found > 0, "wcstombs() accepted all codes 1 to 257, but the locale has 256 characters");
	mid[0] = L'a';
	mid[1] = (wchar_t)found;
	mid[2] = L'b';
	mid[3] = 0;
	errno = EDOM;
	r = wcstombs(buf, mid, sizeof buf);
	e = errno;
	pc_check(r == (size_t)-1, "wcstombs() of a string with invalid code %d inside returned %zu, expected (size_t)-1", found, r);
	pc_check(e == EILSEQ, "wcstombs() of a string with an invalid code set errno to %s, expected EILSEQ", pc_errname(e));
	pc_pass();
}
