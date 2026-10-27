/*
 * dirname() does not depend on what the pathname names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dirname.005
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	static const struct { const char *in, *want; } t[] = {
		{ "link/leaf", "link" },
		{ "file/x", "file" },
		{ "missing/x", "missing" },
	};
	size_t i;

	pc_begin("XSH.dirname.005");
	pc_make_dir("real");
	pc_make_symlink("real", "link");
	pc_make_file("file", "x");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		char buf[32];
		char *r;

		strcpy(buf, t[i].in);
		r = dirname(buf);
		pc_check(r != NULL && strcmp(r, t[i].want) == 0, "dirname(\"%s\") returned \"%s\", expected \"%s\"",
			 t[i].in, r != NULL ? r : "(null)", t[i].want);
	}
	pc_pass();
}
