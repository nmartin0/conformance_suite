/*
 * With FNM_NOESCAPE a backslash is an ordinary character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fnmatch.014
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
	pc_begin("XSH.fnmatch.014");
	pc_test();
	chk("\\*", "\\abc", FNM_NOESCAPE, 1);
	chk("a\\b", "a\\b", FNM_NOESCAPE, 1);
	chk("\\*", "*", FNM_NOESCAPE, 0);
	chk("a\\b", "ab", FNM_NOESCAPE, 0);
	pc_pass();
}
