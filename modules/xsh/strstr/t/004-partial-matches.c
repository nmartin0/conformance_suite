/*
 * strstr: partial matches.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strstr.004
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strstr)(const char *, const char *) = strstr;

int main(void)
{
	static const char h1[] = "aaab", h2[] = "ababac", h3[] = "mississippi", h4[] = "aabaaa";
	pc_begin("XSH.strstr.004");
	pc_test();
	pc_check(f_strstr(h1, "aab") == h1 + 1, "\"aab\" not found at index 1");
	pc_check(f_strstr(h2, "abac") == h2 + 2, "\"abac\" not found at index 2");
	pc_check(f_strstr(h3, "issip") == h3 + 4, "\"issip\" not found at index 4");
	pc_check(f_strstr(h3, "issi") == h3 + 1, "\"issi\" not found at index 1");
	pc_check(f_strstr(h4, "aaa") == h4 + 3, "\"aaa\" not found at index 3");
	pc_pass();
}
