/*
 * strspn: empty operands.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strspn.002
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strspn)(const char *, const char *) = strspn;

int main(void)
{
	pc_begin("XSH.strspn.002");
	pc_test();
	pc_check(f_strspn("abc", "") == 0, "(\"abc\", \"\") did not give 0");
	pc_check(f_strspn("", "abc") == 0, "(\"\", \"abc\") did not give 0");
	pc_check(f_strspn("", "") == 0, "(\"\", \"\") did not give 0");
	pc_pass();
}
