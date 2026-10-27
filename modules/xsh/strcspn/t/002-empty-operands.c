/*
 * strcspn: empty operands.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcspn.002
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strcspn)(const char *, const char *) = strcspn;

int main(void)
{
	pc_begin("XSH.strcspn.002");
	pc_test();
	pc_check(f_strcspn("abc", "") == 3, "(\"abc\", \"\") did not give 3");
	pc_check(f_strcspn("", "") == 0, "(\"\", \"\") did not give 0");
	pc_check(f_strcspn("", "abc") == 0, "(\"\", \"abc\") did not give 0");
	pc_pass();
}
