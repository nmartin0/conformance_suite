/*
 * The white-space characters of the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.3-definitions.002
 */
#include <ctype.h>
#include <locale.h>
#include <wctype.h>
#include "pc.h"

int main(void)
{
	int c;

	pc_begin("XBD.3-definitions.002");
	pc_check(setlocale(LC_ALL, "POSIX") != NULL,
	    "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	for (c = 0; c < 256; c++) {
		int want = (c == ' ' || c == '\t' || c == '\n' || c == '\r' ||
		    c == '\f' || c == '\v');

		pc_check((isspace(c) != 0) == want,
		    "isspace(%d) is %s in the POSIX locale", c,
		    want ? "false" : "true");
		pc_check((iswspace((wint_t)c) != 0) == want,
		    "iswspace(%d) is %s in the POSIX locale", c,
		    want ? "false" : "true");
	}
	pc_pass();
}
