/*
 * The blank characters of the POSIX locale are the space and the tab.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.3-definitions.001
 */
#include <ctype.h>
#include <locale.h>
#include <wctype.h>
#include "pc.h"

int main(void)
{
	int c;

	pc_begin("XBD.3-definitions.001");
	pc_check(setlocale(LC_ALL, "POSIX") != NULL,
	    "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	for (c = 0; c < 256; c++) {
		int want = (c == ' ' || c == '\t');

		pc_check((isblank(c) != 0) == want,
		    "isblank(%d) is %s in the POSIX locale", c,
		    want ? "false" : "true");
		pc_check((iswblank((wint_t)c) != 0) == want,
		    "iswblank(%d) is %s in the POSIX locale", c,
		    want ? "false" : "true");
	}
	pc_pass();
}
