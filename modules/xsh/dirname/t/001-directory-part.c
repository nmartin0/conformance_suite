/*
 * dirname() returns the directory part of a pathname.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dirname.001
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	static const struct { const char *in, *want; } t[] = {
		{ "usr/", "." },
		{ "/usr/", "/" },
		{ "/usr/lib", "/usr" },
		{ "/", "/" },
		{ "a/b", "a" },
		{ "a/b/", "a" },
		{ "a/b/c", "a/b" },
		{ "/a", "/" },
		{ "usr/lib//", "usr" },
		{ "a//b", "a" },
		{ "/home/dwc/.", "/home/dwc" }
	};
	size_t i;

	pc_begin("XSH.dirname.001");
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
