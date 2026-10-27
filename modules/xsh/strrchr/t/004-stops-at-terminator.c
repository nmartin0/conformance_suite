/*
 * strrchr: stops at terminator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strrchr.004
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strrchr)(const char *, int) = strrchr;

int main(void)
{
	static const char s[5] = { 'a', 'b', 0, 'a', 'y' };
	pc_begin("XSH.strrchr.004");
	pc_test();
	pc_check(f_strrchr(s, 'a') == s, "a byte after the terminator was found");
	pc_check(f_strrchr(s, 'y') == NULL, "a byte after the terminator was found (y)");
	pc_pass();
}
