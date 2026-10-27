/*
 * dirname() keeps ".." components that come before the last component.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dirname.003
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	static const struct { const char *in, *want; } t[] = {
		{ "a/../b", "a/.." },
		{ "../x", ".." },
		{ "/../x", "/.." },
		{ "a/b/../c", "a/b/.." }
	};
	size_t i;

	pc_begin("XSH.dirname.003");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		char buf[64];
		char *r;

		strcpy(buf, t[i].in);
		r = dirname(buf);
		pc_check(r != NULL && strcmp(r, t[i].want) == 0, "dirname(\"%s\") returned \"%s\", expected \"%s\"",
			 t[i].in, r != NULL ? r : "(null)", t[i].want);
	}
	pc_pass();
}
