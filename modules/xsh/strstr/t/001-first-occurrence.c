/*
 * strstr: first occurrence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strstr.001
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strstr)(const char *, const char *) = strstr;

int main(void)
{
	static const char s[] = "abcabc";
	pc_begin("XSH.strstr.001");
	pc_test();
	pc_check(strlen(s) == 6, "bad setup");
	pc_check(f_strstr(s, "ca") == s + 2, "\"ca\" not found at index 2");
	pc_check(f_strstr(s, "abc") == s, "\"abc\" not found at index 0");
	pc_check(f_strstr(s, "bc") == s + 1, "\"bc\" not found at index 1");
	pc_check(f_strstr(s, "c") == s + 2, "\"c\" not found at index 2");
	pc_check(f_strstr(s, "abcabc") == s, "the whole string was not found at index 0");
	pc_check(f_strstr(s + 3, "abc") == s + 3, "search from an inner pointer failed");
	pc_check(f_strstr(s, "abd") == NULL, "an absent string was found");
	pc_check(f_strstr(s, "x") == NULL, "an absent byte was found");
	pc_pass();
}
