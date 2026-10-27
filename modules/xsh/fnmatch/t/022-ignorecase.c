/*
 * FNM_IGNORECASE makes the comparison case-insensitive.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fnmatch.022
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
	pc_begin("XSH.fnmatch.022");
	pc_test();
	chk("abc", "ABC", FNM_IGNORECASE, 1);
	chk("ABC", "abc", FNM_IGNORECASE, 1);
	chk("a?c", "AXC", FNM_IGNORECASE, 1);
	chk("[a-c]", "B", FNM_IGNORECASE, 1);
	chk("[[:lower:]]", "A", FNM_IGNORECASE, 1);
	chk("abc", "abd", FNM_IGNORECASE, 0);
	chk("abc", "ABC", 0, 0);
	chk("a?c", "AXC", 0, 0);
	chk("[a-c]", "B", 0, 0);
	chk("[[:lower:]]", "A", 0, 0);
	pc_pass();
}
