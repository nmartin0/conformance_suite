/*
 * memmem: overlapping candidates.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmem.006
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmem)(const void *, size_t, const void *, size_t) = memmem;

int main(void)
{
	pc_begin("XSH.memmem.006");
	pc_test();
	{
		static const char h1[4] = { 'a','a','a','b' }, h2[6] = { 'a','b','a','b','a','c' }, h3[11] = "mississippi", h4[6] = { 'a','a','b','a','a','a' };
		pc_check(f_memmem(h1, 4, "aab", 3) == h1 + 1, "\"aab\" not found at index 1");
		pc_check(f_memmem(h2, 6, "abac", 4) == h2 + 2, "\"abac\" not found at index 2");
		pc_check(f_memmem(h3, 11, "issip", 5) == h3 + 4, "\"issip\" not found at index 4");
		pc_check(f_memmem(h4, 6, "aaa", 3) == h4 + 3, "\"aaa\" not found at index 3");
	}
	pc_pass();
}
