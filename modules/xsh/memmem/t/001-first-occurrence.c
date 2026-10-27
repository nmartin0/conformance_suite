/*
 * memmem: first occurrence.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmem.001
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmem)(const void *, size_t, const void *, size_t) = memmem;

int main(void)
{
	static const char h[9] = { 'a','b','c','a','b','c','a','b','c' };
	pc_begin("XSH.memmem.001");
	pc_test();
	pc_check(f_memmem(h, 9, "cab", 3) == h + 2, "\"cab\" not found at index 2");
	pc_check(f_memmem(h, 9, "abc", 3) == h, "\"abc\" not found at index 0");
	pc_check(f_memmem(h, 9, "bc", 2) == h + 1, "\"bc\" not found at index 1");
	pc_check(f_memmem(h, 9, "c", 1) == h + 2, "\"c\" not found at index 2");
	pc_check(f_memmem(h + 3, 6, "abc", 3) == h + 3, "search from an inner pointer failed");
	pc_check(f_memmem(h, 9, "abd", 3) == NULL, "an absent sequence was found");
	pc_check(f_memmem(h, 9, "x", 1) == NULL, "an absent byte was found");
	pc_pass();
}
