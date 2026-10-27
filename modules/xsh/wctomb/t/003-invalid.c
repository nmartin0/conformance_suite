/*
 * wctomb() fails with EILSEQ for a code that is no character; the POSIX
 * locale has 256 characters, so among 257 distinct codes at least one is
 * invalid. At most MB_CUR_MAX bytes are stored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wctomb.003
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char buf[32];
	size_t max = MB_CUR_MAX;
	int v, r, e, nbad = 0, badv = -1, badr = 0, bade = 0, over = -1;

	pc_begin("XSH.wctomb.003");
	pc_check(max >= 1 && max < sizeof buf - 1, "setup: MB_CUR_MAX is %zu", max);
	pc_test();
	for (v = 0; v <= 256; v++) {
		memset(buf, 'x', sizeof buf);
		errno = EDOM;
		r = wctomb(buf, (wchar_t)v);
		e = errno;
		if (r == -1) {
			nbad++;
			if (e != EILSEQ && bade == 0)
				bade = e;
		} else if (r != 1 && badv < 0) {
			badv = v;
			badr = r;
		}
		if (over < 0 && buf[max] != 'x')
			over = v;
	}
	pc_check(badv < 0, "wctomb(%d) returned %d, expected 1 or -1", badv, badr);
	pc_check(bade == 0, "wctomb() failed with errno %s, expected EILSEQ", pc_errname(bade));
	pc_check(nbad > 0, "wctomb() accepted all 257 codes 0 to 256, but the locale has 256 characters");
	pc_check(over < 0, "wctomb(%d) stored more than MB_CUR_MAX bytes", over);
	pc_pass();
}
