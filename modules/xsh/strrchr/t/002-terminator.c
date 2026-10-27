/*
 * strrchr: terminator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strrchr.002
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strrchr)(const char *, int) = strrchr;

int main(void)
{
	static const char s[] = "abcabc", e[] = "";
	pc_begin("XSH.strrchr.002");
	pc_test();
	pc_check(f_strrchr(s, 0) == s + 6, "the terminator was not found");
	pc_check(f_strrchr(e, 0) == e, "the terminator of the empty string was not found");
	pc_check(f_strrchr(e, 'a') == NULL, "a byte was found in the empty string");
	pc_pass();
}
