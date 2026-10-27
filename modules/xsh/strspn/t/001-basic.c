/*
 * strspn: basic.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strspn.001
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strspn)(const char *, const char *) = strspn;

int main(void)
{
	pc_begin("XSH.strspn.001");
	pc_test();
	pc_check(f_strspn("abcabcd", "abc") == 6, "(\"abcabcd\", \"abc\") did not give 6");
	pc_check(f_strspn("abc", "cba") == 3, "(\"abc\", \"cba\") did not give 3");
	pc_check(f_strspn("aab", "a") == 2, "(\"aab\", \"a\") did not give 2");
	pc_check(f_strspn("xyz", "abc") == 0, "(\"xyz\", \"abc\") did not give 0");
	pc_check(f_strspn("12 34", "0123456789") == 2, "(\"12 34\", digits) did not give 2");
	pc_pass();
}
