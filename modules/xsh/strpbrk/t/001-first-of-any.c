/*
 * strpbrk: first of any.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strpbrk.001
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strpbrk)(const char *, const char *) = strpbrk;

int main(void)
{
	static const char s[] = "abcdef", t[] = "abc", u[] = "a b";
	pc_begin("XSH.strpbrk.001");
	pc_test();
	pc_check(f_strpbrk(s, "fd") == s + 3, "(\"abcdef\", \"fd\") did not give index 3");
	pc_check(f_strpbrk(t, "a") == t, "(\"abc\", \"a\") did not give index 0");
	pc_check(f_strpbrk(t, "cbx") == t + 1, "(\"abc\", \"cbx\") did not give index 1");
	pc_check(f_strpbrk(u, " ") == u + 1, "(\"a b\", \" \") did not give index 1");
	pc_pass();
}
