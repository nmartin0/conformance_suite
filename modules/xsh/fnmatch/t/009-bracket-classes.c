/*
 * Character class expressions match their class in the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fnmatch.009
 */
#include "pc.h"
#include <errno.h>
#include <fnmatch.h>
#include <stdio.h>
#include <string.h>

/* The result must be 0 (match) or FNM_NOMATCH (no match). */
static void chk(const char *p, const char *s, int fl, int match)
{
	int r = fnmatch(p, s, fl);

	if (match)
		pc_check(r == 0, "fnmatch(\"%s\", \"%s\", %#x) returned %d, expected 0", p, s, (unsigned)fl, r);
	else
		pc_check(r == FNM_NOMATCH, "fnmatch(\"%s\", \"%s\", %#x) returned %d, expected FNM_NOMATCH", p, s, (unsigned)fl, r);
}

int main(void)
{
	static const struct { const char *name, *in, *out; } c[] = {
		{ "alpha", "a", "1" },
		{ "digit", "5", "a" },
		{ "upper", "A", "a" },
		{ "lower", "a", "A" },
		{ "alnum", "z", "_" },
		{ "space", " ", "a" },
		{ "blank", "\t", "a" },
		{ "punct", "!", "a" },
		{ "xdigit", "f", "g" },
		{ "cntrl", "\001", "a" },
		{ "print", "a", "\001" },
		{ "graph", "a", " " },
	};
	char p[32];
	size_t i;

	pc_begin("XSH.fnmatch.009");
	pc_test();
	for (i = 0; i < sizeof c / sizeof c[0]; i++) {
		snprintf(p, sizeof p, "[[:%s:]]", c[i].name);
		chk(p, c[i].in, 0, 1);
		chk(p, c[i].out, 0, 0);
	}
	pc_pass();
}
