/*
 * The '^' flag leaves out grouping.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.008
 */
#include "fmon.h"

static void run(const char *w)
{
	static const char *const fmt[2] = { "%^.0n", "%^.0i" };
	char buf[128];
	int i;

	for (i = 0; i < 2; i++) {
		fm_ok(buf, sizeof buf, fmt[i], 1234567.0);
		pc_check(strstr(buf, "1234567") != NULL, "%s: strfmon(\"%s\", 1234567.0) gave \"%s\", the digits are not one unbroken run", w, fmt[i], buf);
	}
}

int main(void)
{
	int have;

	pc_begin("XSH.strfmon.008");
	lc_init();
	have = lc_find();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	run("POSIX locale");
	if (have) {
		if (setlocale(LC_ALL, lc_loc) == NULL)
			pc_unresolved("could not select the second locale");
		run("second locale");
		(void)setlocale(LC_ALL, "C");
	}
	pc_pass();
}
