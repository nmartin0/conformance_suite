/*
 * strpbrk: none found.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strpbrk.002
 */
#include "pc.h"
#include <string.h>
static char *(*volatile f_strpbrk)(const char *, const char *) = strpbrk;

int main(void)
{
	pc_begin("XSH.strpbrk.002");
	pc_test();
	pc_check(f_strpbrk("abc", "xyz") == NULL, "(\"abc\", \"xyz\") did not give null");
	pc_check(f_strpbrk("abc", "") == NULL, "(\"abc\", \"\") did not give null");
	pc_check(f_strpbrk("", "abc") == NULL, "(\"\", \"abc\") did not give null");
	pc_check(f_strpbrk("", "") == NULL, "(\"\", \"\") did not give null");
	pc_pass();
}
