/*
 * basename() of a string of slashes only returns "/".
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.basename.002
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	static const struct { const char *in, *want; } t[] = {
		{ "/", "/" },
		{ "///", "/" },
		{ "//////", "/" }
	};
	size_t i;

	pc_begin("XSH.basename.002");
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
