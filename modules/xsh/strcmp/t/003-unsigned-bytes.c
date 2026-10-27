/*
 * strcmp: unsigned bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcmp.003
 */
#include "pc.h"
#include <string.h>
static int (*volatile f_strcmp)(const char *, const char *) = strcmp;

int main(void)
{
	static const char s1[] = { 0x7f, 0 }, s2[] = { (char)0x80, 0 }, s3[] = { (char)0xff, 0 }, s4[] = { 'a', (char)0xe9, 0 }, s5[] = { 'a', 'b', 0 };
	pc_begin("XSH.strcmp.003");
	pc_test();
	pc_check(f_strcmp(s1, s2) < 0, "0x7f was not less than 0x80");
	pc_check(f_strcmp(s2, s1) > 0, "0x80 was not greater than 0x7f");
	pc_check(f_strcmp(s3, "a") > 0, "0xff was not greater than a");
	pc_check(f_strcmp(s4, s5) > 0, "a 0xe9 byte was not greater than b");
	pc_pass();
}
