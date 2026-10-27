/*
 * dirname() returns "." for a null pointer, an empty string and a name without a slash.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dirname.004
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	static const char *const in[] = { "", "usr", "x.y", ".", ".." };
	size_t i;
	char *r;

	pc_begin("XSH.dirname.004");
	pc_test();
	r = dirname(NULL);
	pc_check(r != NULL && strcmp(r, ".") == 0, "dirname(NULL) returned \"%s\"", r != NULL ? r : "(null)");
	for (i = 0; i < sizeof in / sizeof in[0]; i++) {
		char buf[16];

		strcpy(buf, in[i]);
		r = dirname(buf);
		pc_check(r != NULL && strcmp(r, ".") == 0, "dirname(\"%s\") returned \"%s\", expected \".\"", in[i], r != NULL ? r : "(null)");
	}
	pc_pass();
}
