/*
 * strrchr: high byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strrchr.003
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strrchr)(const char *, int) = strrchr;

int main(void)
{
	static const char s[4] = { (char)0xe9, 'a', (char)0xe9, 0 };
	char c = (char)0xe9;
	pc_begin("XSH.strrchr.003");
	pc_test();
	pc_check(f_strrchr(s, c) == s + 2, "the last 0xe9 byte was not found at index 2");
	pc_pass();
}
