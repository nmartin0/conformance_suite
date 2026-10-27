/*
 * memmem: haystack shorter.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmem.004
 */
#include "pc.h"
#include <string.h>
static void *(*volatile f_memmem)(const void *, size_t, const void *, size_t) = memmem;

int main(void)
{
	static const char h[5] = "abcd";
	pc_begin("XSH.memmem.004");
	pc_test();
	pc_check(f_memmem(h, 3, "abcd", 4) == NULL, "a needle longer than the haystack was found");
	pc_check(f_memmem(h, 0, "a", 1) == NULL, "a needle was found in an empty haystack");
	pc_check(f_memmem(h, 4, "abcd", 4) == h, "equal sequences were not found");
	pc_check(f_memmem(h, 4, "abce", 4) == NULL, "different sequences of equal length were found");
	pc_pass();
}
