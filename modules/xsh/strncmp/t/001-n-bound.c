/*
 * strncmp: n bound.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncmp.001
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_strncmp)(const char *, const char *, size_t) = strncmp;

int main(void)
{
	pc_begin("XSH.strncmp.001");
	pc_test();
	pc_check(f_strncmp("abcX", "abcY", 3) == 0, "a byte after n made the strings differ");
	pc_check(f_strncmp("abcX", "abcY", 4) < 0, "the fourth byte was not compared");
	pc_check(f_strncmp("abc", "abd", 2) == 0, "a byte after n made the strings differ (2)");
	pc_check(f_strncmp("abc", "abd", 3) < 0, "the third byte was not compared");
	pc_check(f_strncmp("abc", "abd", 1000) < 0, "n larger than the strings failed");
	pc_pass();
}
