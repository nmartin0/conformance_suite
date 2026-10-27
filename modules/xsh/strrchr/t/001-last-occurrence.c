/*
 * strrchr: last occurrence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strrchr.001
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strrchr)(const char *, int) = strrchr;

int main(void)
{
	static const char s[] = "abcabc", t[] = "abcd";
	pc_begin("XSH.strrchr.001");
	pc_test();
	pc_check(f_strrchr(s, 'b') == s + 4, "last b not found at index 4");
	pc_check(f_strrchr(s, 'a') == s + 3, "last a not found at index 3");
	pc_check(f_strrchr(s, 'c') == s + 5, "last c not found at index 5");
	pc_check(f_strrchr(s, 'z') == NULL, "an absent byte was reported present");
	pc_check(f_strrchr(t, 'd') == t + 3, "a single byte was not found");
	pc_check(f_strrchr(t, 'a') == t, "the first byte was not found");
	pc_pass();
}
