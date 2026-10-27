/*
 * A [ that starts no valid bracket expression matches itself.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fnmatch.012
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
	pc_begin("XSH.fnmatch.012");
	pc_test();
	chk("[", "[", 0, 1);
	chk("[a", "[a", 0, 1);
	chk("[a", "a", 0, 0);
	pc_pass();
}
