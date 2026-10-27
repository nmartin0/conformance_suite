/*
 * strcspn: high bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcspn.003
 */
#include "pc.h"
#include <string.h>
static size_t (*volatile f_strcspn)(const char *, const char *) = strcspn;

int main(void)
{
	static const char s1[] = { (char)0xe9, 'a', 0 }, s2[] = { 'a', (char)0xe9, 0 }, s3[] = { 'a', (char)0xff, 0 }, set[] = { (char)0xe9, 0 };
	pc_begin("XSH.strcspn.003");
	pc_test();
	pc_check(f_strcspn(s1, set) == 0, "a leading high byte in the set gave the wrong length");
	pc_check(f_strcspn(s2, set) == 1, "a high byte in the set at index 1 gave the wrong length");
	pc_check(f_strcspn(s3, set) == 2, "a different high byte was taken as a member");
	pc_pass();
}
