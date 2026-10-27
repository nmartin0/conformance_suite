/*
 * dirname() returns one of the results the page allows where it allows several.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dirname.002
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	static const struct { const char *in, *a, *b; } t[] = {
		{ "//", "/", "//" },
		{ "///", "/", "///" },
		{ "//foo", "//", "/" },
		{ "//usr//lib//", "//usr", "/usr" },
		{ "/home//dwc//test", "/home//dwc", "/home/dwc" },
		{ "/home/.././test", "/home/../.", "/home/.." },
		{ "a/./b", "a/.", "a" },
	};
	size_t i;

	pc_begin("XSH.dirname.002");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		char buf[64];
		char *r;

		strcpy(buf, t[i].in);
		r = dirname(buf);
		pc_check(r != NULL && (strcmp(r, t[i].a) == 0 || strcmp(r, t[i].b) == 0),
			 "dirname(\"%s\") returned \"%s\", expected \"%s\" or \"%s\"",
			 t[i].in, r != NULL ? r : "(null)", t[i].a, t[i].b);
	}
	pc_pass();
}
