/*
 * strncmp: unsigned sign.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncmp.003
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_strncmp)(const char *, const char *, size_t) = strncmp;

int main(void)
{
	static const char s1[] = { 0x7f, 0 }, s2[] = { (char)0x80, 0 }, s3[] = { (char)0xff, 0 };
	pc_begin("XSH.strncmp.003");
	pc_test();
	pc_check(f_strncmp(s1, s2, 1) < 0, "0x7f was not less than 0x80");
	pc_check(f_strncmp(s2, s1, 1) > 0, "0x80 was not greater than 0x7f");
	pc_check(f_strncmp(s3, "a", 1) > 0, "0xff was not greater than a");
	pc_check(f_strncmp("a9", "b0", 2) < 0, "a later pair decided over the first differing pair");
	pc_pass();
}
