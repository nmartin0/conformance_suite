/*
 * strchr: high byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strchr.003
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strchr)(const char *, int) = strchr;

int main(void)
{
	static const char s[5] = { 'a', (char)0xe9, 'b', (char)0xff, 0 };
	char c1 = (char)0xe9, c2 = (char)0xff;
	pc_begin("XSH.strchr.003");
	pc_test();
	pc_check(f_strchr(s, c1) == s + 1, "the 0xe9 byte was not found at index 1");
	pc_check(f_strchr(s, c2) == s + 3, "the 0xff byte was not found at index 3");
	pc_pass();
}
