/*
 * strchr: first occurrence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strchr.001
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strchr)(const char *, int) = strchr;

int main(void)
{
	static const char s[] = "abcabc";
	pc_begin("XSH.strchr.001");
	pc_test();
	pc_check(f_strchr(s, 'b') == s + 1, "first b not found at index 1");
	pc_check(f_strchr(s, 'a') == s, "first a not found at index 0");
	pc_check(f_strchr(s, 'c') == s + 2, "first c not found at index 2");
	pc_check(f_strchr(s, 'z') == NULL, "an absent byte was reported present");
	pc_check(f_strchr(s + 3, 'b') == s + 4, "search from an inner pointer failed");
	pc_pass();
}
