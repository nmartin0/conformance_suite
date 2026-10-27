/*
 * strncmp: n zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncmp.004
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_strncmp)(const char *, const char *, size_t) = strncmp;

int main(void)
{
	pc_begin("XSH.strncmp.004");
	pc_test();
	pc_check(f_strncmp("abc", "xyz", 0) == 0, "n 0 did not compare equal");
	pc_check(f_strncmp("", "xyz", 0) == 0, "n 0 did not compare equal (empty)");
	pc_pass();
}
