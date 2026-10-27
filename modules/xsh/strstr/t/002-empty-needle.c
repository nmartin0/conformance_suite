/*
 * strstr: empty needle.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strstr.002
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strstr)(const char *, const char *) = strstr;

int main(void)
{
	static const char s[] = "abc", e[] = "";
	pc_begin("XSH.strstr.002");
	pc_test();
	pc_check(f_strstr(s, "") == s, "an empty needle did not give the haystack");
	pc_check(f_strstr(e, "") == e, "an empty needle in an empty haystack did not give the haystack");
	pc_check(f_strstr(s + 2, "") == s + 2, "an empty needle did not give an inner haystack pointer");
	pc_pass();
}
