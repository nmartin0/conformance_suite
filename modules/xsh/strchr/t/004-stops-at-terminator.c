/*
 * strchr: stops at terminator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strchr.004
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strchr)(const char *, int) = strchr;

int main(void)
{
	static const char s[5] = { 'a', 'b', 0, 'x', 'y' };
	pc_begin("XSH.strchr.004");
	pc_test();
	pc_check(f_strchr(s, 'x') == NULL, "a byte after the terminator was found");
	pc_check(f_strchr(s, 'b') == s + 1, "a byte before the terminator was not found");
	pc_pass();
}
