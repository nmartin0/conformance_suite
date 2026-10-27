/*
 * Plain text and %% are copied; unused arguments are ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strfmon.001
 */
#include "fmon.h"

int main(void)
{
	static const char *const fmt[4] = { "abc", "100%%", "a%%b%%", "" };
	static const char *const want[4] = { "abc", "100%", "a%b%", "" };
	char buf[64];
	ssize_t r;
	int i;

	pc_begin("XSH.strfmon.001");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	for (i = 0; i < 4; i++) {
		memset(buf, 'X', sizeof buf);
		r = strfmon(buf, sizeof buf, fmt[i], 1.5, 2.5);
		pc_check(r == (ssize_t)strlen(want[i]), "strfmon(\"%s\") returned %ld, expected %zu", fmt[i], (long)r, strlen(want[i]));
		pc_check(strcmp(buf, want[i]) == 0, "strfmon(\"%s\") gave \"%s\", expected \"%s\"", fmt[i], buf, want[i]);
	}
	pc_pass();
}
