/*
 * A bracket list or range matches one character of it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fnmatch.005
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
	pc_begin("XSH.fnmatch.005");
	pc_test();
	chk("[abc]", "b", 0, 1);
	chk("[a-c]", "b", 0, 1);
	chk("[%--]", "+", 0, 1);
	chk("[--@]", "0", 0, 1);
	chk("[abc]", "d", 0, 0);
	chk("[a-c]", "d", 0, 0);
	chk("[abc]", "ab", 0, 0);
	pc_pass();
}
