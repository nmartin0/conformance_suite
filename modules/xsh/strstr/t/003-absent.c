/*
 * strstr: absent.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strstr.003
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strstr)(const char *, const char *) = strstr;

int main(void)
{
	static const char s[] = "abc";
	pc_begin("XSH.strstr.003");
	pc_test();
	pc_check(f_strstr(s, "abcd") == NULL, "a longer needle was found");
	pc_check(f_strstr("", "a") == NULL, "a needle was found in an empty string");
	pc_check(f_strstr(s, "cd") == NULL, "a needle running past the end was found");
	pc_check(f_strstr(s, "c") == s + 2, "the final byte was not found");
	pc_pass();
}
