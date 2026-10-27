/*
 * strncmp: stops at terminator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncmp.002
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_strncmp)(const char *, const char *, size_t) = strncmp;

int main(void)
{
	static const char x[4] = { 'a', 'b', 0, 'X' }, y[4] = { 'a', 'b', 0, 'Y' };
	pc_begin("XSH.strncmp.002");
	pc_test();
	pc_check(f_strncmp(x, y, 4) == 0, "bytes after the terminator were compared");
	pc_check(f_strncmp("ab", "ab", 100) == 0, "equal strings with large n did not compare equal");
	pc_check(f_strncmp("ab", "abc", 3) < 0, "a terminator was not less than a byte");
	pc_check(f_strncmp("abc", "ab", 3) > 0, "a byte was not greater than a terminator");
	pc_pass();
}
