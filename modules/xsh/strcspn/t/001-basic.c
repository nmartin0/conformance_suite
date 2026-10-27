/*
 * strcspn: basic.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcspn.001
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strcspn)(const char *, const char *) = strcspn;

int main(void)
{
	pc_begin("XSH.strcspn.001");
	pc_test();
	pc_check(f_strcspn("abcdef", "dc") == 2, "(\"abcdef\", \"dc\") did not give 2");
	pc_check(f_strcspn("abc", "a") == 0, "(\"abc\", \"a\") did not give 0");
	pc_check(f_strcspn("abc", "xyz") == 3, "(\"abc\", \"xyz\") did not give 3");
	pc_check(f_strcspn("hello world", " ") == 5, "(\"hello world\", \" \") did not give 5");
	pc_check(f_strcspn("a,b;c", ";,") == 1, "(\"a,b;c\", \";,\") did not give 1");
	pc_check(f_strcspn("abc", "cba") == 0, "(\"abc\", \"cba\") did not give 0");
	pc_pass();
}
