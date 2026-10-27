/*
 * basename() returns the final component without trailing slashes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.basename.001
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	static const struct { const char *in, *want; } t[] = {
		{ "usr", "usr" },
		{ "usr/", "usr" },
		{ "/usr/", "usr" },
		{ "/usr/lib", "lib" },
		{ "//usr//lib//", "lib" },
		{ "/home//dwc//test", "test" },
		{ "/home/.././test", "test" },
		{ "a/b///", "b" },
		{ "///x", "x" },
		{ "x.y", "x.y" },
		{ "/home/dwc/.", "." }
	};
	size_t i;

	pc_begin("XSH.basename.001");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		char buf[64];
		char *r;

		strcpy(buf, t[i].in);
		r = basename(buf);
		pc_check(r != NULL && strcmp(r, t[i].want) == 0, "basename(\"%s\") returned \"%s\", expected \"%s\"",
			 t[i].in, r != NULL ? r : "(null)", t[i].want);
	}
	pc_pass();
}
