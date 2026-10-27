/*
 * * matches any string, including the empty one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fnmatch.004
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
	pc_begin("XSH.fnmatch.004");
	pc_test();
	chk("a*c", "ac", 0, 1);
	chk("a*c", "abbbc", 0, 1);
	chk("*", "", 0, 1);
	chk("a*b*c", "aXbYbZc", 0, 1);
	chk("a*", "b", 0, 0);
	chk("a*c", "abd", 0, 0);
	chk("*b", "abxb", 0, 1);
	pc_pass();
}
